#pragma once

/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

GeneralResult setupDrive();

/**
 * @param left_wheel_rotation in range [-1.0; 1.0]
 *        with -1.0 = back, 0.0 = stop, 1.0 = forward.
 * @param right_wheel_rotation in range [-1.0; 1.0]
 *        with -1.0 = back, 0.0 = stop, 1.0 = forward.
 */
GeneralResult setDriveSpeed(float left_wheel_rotation,
                            float right_wheel_rotation);
