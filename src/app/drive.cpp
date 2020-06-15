/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

#include <math.h>

#include "config.h"
#include "Arduino.h"


#define DRIVE_PWM_RANGE         (DRIVE_PWM_FASTEST_VALUE - DRIVE_PWM_STOP_VALUE)
#define DRIVE_PWM_MIN           0
#define DRIVE_PWM_MAX           254
#define DRIVE_PWM_FLOAT_ZERO    0.001f


namespace
{

struct PWMValues
{
    int left  = 0;
    int right = 0;
};

float toPurePWMValue(float value)
{
    value = fabs(value);
    if (value < DRIVE_PWM_FLOAT_ZERO)
    {
        return DRIVE_PWM_MIN;
    }
    return (value * DRIVE_PWM_RANGE) + DRIVE_PWM_STOP_VALUE;
}

PWMValues toPWMValues(const float left_value, const float right_value)
{
    PWMValues result;
    result.left = (int)roundf(toPurePWMValue(left_value));
    result.left = constrain(result.left, DRIVE_PWM_MIN, DRIVE_PWM_MAX);
    float result_right = toPurePWMValue(right_value);
    result_right = DRIVE_PWM_RIGHT_ADJUST_A * (result_right * result_right) +
            DRIVE_PWM_RIGHT_ADJUST_B * result_right + DRIVE_PWM_RIGHT_ADJUST_C;
    result.right = constrain((int)roundf(result_right), DRIVE_PWM_MIN, DRIVE_PWM_MAX);
    return result;
}

}

GeneralResult setupDrive()
{
    pinMode(DRIVE_LEFT_CONTROL_PIN1,  OUTPUT);
    pinMode(DRIVE_LEFT_CONTROL_PIN2,  OUTPUT);
    pinMode(DRIVE_RIGHT_CONTROL_PIN1, OUTPUT);
    pinMode(DRIVE_RIGHT_CONTROL_PIN1, OUTPUT);
    pinMode(DRIVE_LEFT_PWM_PIN,       OUTPUT);
    pinMode(DRIVE_RIGHT_PWM_PIN,      OUTPUT);

    digitalWrite(DRIVE_LEFT_PWM_PIN,  LOW);
    digitalWrite(DRIVE_RIGHT_PWM_PIN, LOW);

    return GeneralResult::Ok;
}

GeneralResult setDriveSpeed(float left_wheel_rotation,
                            float right_wheel_rotation)
{
    PWMValues pwm = toPWMValues(left_wheel_rotation, right_wheel_rotation);
    uint8_t left_control_val1 = LOW;
    uint8_t left_control_val2 = LOW;
    uint8_t right_control_val1 = LOW;
    uint8_t right_control_val2 = LOW;

    if (pwm.left != 0)
    {
        if (left_wheel_rotation >= 0.0f)
        {
            left_control_val1 = HIGH;
            left_control_val2 = LOW;
        }
        else
        {
            left_control_val1 = LOW;
            left_control_val2 = HIGH;
        }
    }

    if (pwm.right != 0)
    {
        if (right_wheel_rotation >= 0.0f)
        {
            right_control_val1 = LOW;
            right_control_val2 = HIGH;
        }
        else
        {
            right_control_val1 = HIGH;
            right_control_val2 = LOW;
        }
    }

    analogWrite(DRIVE_LEFT_PWM_PIN, pwm.left);
    analogWrite(DRIVE_RIGHT_PWM_PIN, pwm.right);
    digitalWrite(DRIVE_LEFT_CONTROL_PIN1, left_control_val1);
    digitalWrite(DRIVE_LEFT_CONTROL_PIN2, left_control_val2);
    digitalWrite(DRIVE_RIGHT_CONTROL_PIN1, right_control_val1);
    digitalWrite(DRIVE_RIGHT_CONTROL_PIN2, right_control_val2);

    return GeneralResult::Ok;
}
