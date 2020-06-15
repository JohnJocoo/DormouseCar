/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

#include "drive_control_task.h"

#include <string.h>
#include "drive_direction_setting.h"
#include "relative_time.h"
#include "allocate.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/queue.h"


enum class DriveCommandType : uint8_t
{
    NOP,
    SET_PLAN,
    BREAK
};

struct DriveCommand
{
    DriveCommandType type;
    DriveDirectionSetting* plan;
    unsigned int planSize;

    DriveCommand(DriveCommandType type);

};


DriveDirectionSetting drive_settings[DRIVE_SETTINGS_SIZE];
unsigned int drive_settings_size = 0;
int current_drive_setting = -1;
QueueHandle_t drive_commands_queue;


static void run(void*);


DriveCommand::DriveCommand(DriveCommandType type)
: type{type}
, plan{nullptr}
, {0}
{}

GeneralResult setupDriveControlTask()
{
    drive_commands_queue = xQueueCreate(DRIVE_TASK_QUEUE_SZ,
                                        sizeof(DriveCommand));

    if (drive_commands_queue == NULL)
    {
        return GeneralResult::ErrorNoMemory;
    }

    return GeneralResult::Ok;
}

static GeneralResult getDriveCommandFromPlan(
        const DriveDirectionSetting* plan, unsigned int size,
        DriveCommand& result)
{
    if (plan == nullptr || size == 0)
    {
        return GeneralResult::Ok;
    }

    void* buff = os_raw_alloc(size * sizeof(DriveDirectionSetting));

    if (buff == nullptr)
    {
        return GeneralResult::ErrorNoMemory;
    }

    memcpy(buff, plan, size * sizeof(DriveDirectionSetting));
    cmd.plan = (DriveDirectionSetting*)buff;
    cmd.planSize = size;

    return GeneralResult::Ok;
}

GeneralResult setDrivePlan(const DriveDirectionSetting* plan, unsigned int size)
{
    if (size > DRIVE_SETTINGS_SIZE)
    {
        return GeneralResult::ErrorOverflow;
    }

    DriveCommand cmd{DriveCommandType::SET_PLAN};
    GeneralResult resultCmd = getDriveCommandFromPlan(plan, size, cmd);
    if (resultCmd != GeneralResult::Ok)
    {
        return resultCmd;
    }

    if (pdTRUE != xQueueSend(drive_commands_queue, &cmd, 10))
    {
        os_raw_dealloc(cmd.plan);

        return GeneralResult::ErrorTimeout;
    }

    return GeneralResult::Ok;
}

GeneralResult breakNow()
{
    DriveCommand cmd{DriveCommandType::BREAK};

    if (pdTRUE != xQueueSend(drive_commands_queue, &cmd, 10))
    {
        return GeneralResult::ErrorTimeout;
    }

    return GeneralResult::Ok;
}

static void setDrivePlanImpl(const DriveDirectionSetting* plan, unsigned int size)
{
    if (plan == nullptr || size == 0)
    {
        drive_settings_size = 0;
        current_drive_setting = -1;

        return;
    }

    if (size > DRIVE_SETTINGS_SIZE)
    {
        return;
    }

    memcpy(drive_settings, plan, size * sizeof(DriveDirectionSetting));
    drive_settings_size = size;
    current_drive_setting = -1;
}

static void breakNowImpl()
{
    // TODO check if going opposite direction will break faster
    drive_settings[0] = DriveDirectionSetting{};
    drive_settings_size = 1;
    current_drive_setting = -1;
}

static int updateCurrentSetting(const Duration& time_now)
{
    int last_current_drive_setting = current_drive_setting;
    for (int i = current_drive_setting + 1; i < DRIVE_SETTINGS_SIZE; ++i)
    {
        DriveDirectionSetting& setting = drive_settings[i];

        if (setting.startAtTimeFromEpoch.isInvalid() ||
            setting.startAtTimeFromEpoch <= time_now)
        {
            current_drive_setting = i;
            continue;
        }
        break;
    }

    if (current_drive_setting == last_current_drive_setting)
    {
        return current_drive_setting;
    }

    if (current_drive_setting >= 0 &&
        drive_settings[current_drive_setting].startAtTimeFromEpoch.isInvalid())
    {
        drive_settings[current_drive_setting].startAtTimeFromEpoch = time_now;
    }

    return current_drive_setting;
}

static DriveDirectionSetting getCurrentSetting(const Duration& time_now)
{
    if (drive_settings_size == 0)
    {
        return DriveDirectionSetting{};
    }

    int i = updateCurrentSetting(time_now);

    if (i < 0)
    {
        return DriveDirectionSetting{};
    }

    return drive_settings[i];
}

static void driveStep()
{
    Duration time_now = Duration::timeFromEpoch();
    DriveDirectionSetting setting = getCurrentSetting(time_now);

    float seconds = (time_now - setting.startAtTimeFromEpoch).millisTotal() / 1000.0f;

    // TODO: test what pwm values will give what speed and how to set angle by different rotation speed
}

void run(void*)
{
    DriveCommand cmd;
    TickType_t delay = Duration::fromMillis(DRIVE_TASK_SLEEP_MS).toOSTicks();

    for (;;)
    {
        driveStep();

        if (pdTRUE == xQueueReceive(drive_commands_queue, &cmd, delay))
        {
            switch (cmd.type)
            {
                case DriveCommandType::SET_PLAN :
                    setDrivePlanImpl(cmd.plan, cmd.planSize);
                    os_raw_dealloc(cmd.plan);
                    break;
                case DriveCommandType::BREAK :
                    breakNowImpl();
                    break;
                default:
                    break;
            }
        }
    }
}
