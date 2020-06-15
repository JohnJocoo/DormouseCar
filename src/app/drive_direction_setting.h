#pragma once

/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

#include "relative_time.h"


struct DriveDirectionSetting
{
    float startVelocity; // millimeters / sec
    float velocityAcceleration; // millimeters / sec^2

    float startAngleVelocity; // rads / sec
    float angleAcceleration; // rads / sec^2

    Duration startAtTimeFromEpoch;

    DriveDirectionSetting();

}; // struct DriveDirectionSetting


DriveDirectionSetting::DriveDirectionSetting()
: startVelocity{0.0f}
, velocityAcceleration{0.0f}
, startAngleVelocity{0.0f}
, angleAcceleration{0.0f}
, startAtTimeFromEpoch{Duration::invalid()}
{

}
