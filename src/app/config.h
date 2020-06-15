#pragma once

/**
 * @file
 * @copyright (C) Anton Frolov johnjocoo@gmail.com
 */

// ==== Motor driver, moving conf ====

#define DRIVE_LEFT_PWM_PIN       5
#define DRIVE_RIGHT_PWM_PIN      6
#define DRIVE_LEFT_CONTROL_PIN1  7
#define DRIVE_LEFT_CONTROL_PIN2  8
#define DRIVE_RIGHT_CONTROL_PIN1 9
#define DRIVE_RIGHT_CONTROL_PIN1 11

#define DRIVE_SETTINGS_SIZE      5

#define DRIVE_PWM_STOP_VALUE    6.12116041e+01f
#define DRIVE_PWM_FASTEST_VALUE 250.0f

#define DRIVE_PWM_RIGHT_ADJUST_A 4.27557032e-04f
#define DRIVE_PWM_RIGHT_ADJUST_B 6.69504220e-01f
#define DRIVE_PWM_RIGHT_ADJUST_C 9.12775303e+00f

#define DRIVE_SPEED_TO_NORM_VALUE_A 1.92752046e-06f
#define DRIVE_SPEED_TO_NORM_VALUE_B 1.87885540e-04f

// ==== Lidar/Sonar sensor and servo control ====

#define LIDAR_CONF_MIN_ANGLE 700
#define LIDAR_CONF_MAX_ANGLE 2400
#define LIDAR_SERVO_PIN      10
#define LIDAR_START_ANGLE    90
#define LIDAR_ANGLE_INC      10

// ==== FreeRTOS tasks configuration ====

#define DRIVE_STACK_SIZE    128
#define DRIVE_TASK_PRIORITY 2
#define DRIVE_TASK_SLEEP_MS 50
#define DRIVE_TASK_QUEUE_SZ 8

#define LIDAR_STACK_SIZE    128
#define LIDAR_TASK_PRIORITY 3


enum class GeneralResult
{
    Ok,
    ErrorGeneral,
    ErrorNoMemory,
    ErrorNotSupported,
    ErrorArgument,
    ErrorDomain,
    ErrorTimeout,
    ErrorOverflow,
    ErrorUnderflow,
    ErrorUnknown
};
