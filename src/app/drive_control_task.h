#pragma once

/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

#include "config.h"
#include "drive_direction_setting.h"


GeneralResult setupDriveControlTask();

GeneralResult setDrivePlan(const DriveDirectionSetting* plan, unsigned int size);
GeneralResult breakNow();
