#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "Arduino.h"
#include "PWMServo.h"

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

#define MIN_ANGLE 700
#define MAX_ANGLE 2400
#define SERVO_PIN 10

#define DRIVE_STACK_SIZE 128
#define LIDAR_STACK_SIZE 128

#define DRIVE_TASK_PRIORITY 2
#define LIDAR_TASK_PRIORITY 3

#define START_LIDAR_ANGLE 90
#define LIDAR_ANGLE_INC 10

void setup();
void run();

void move(void*);
void lidar(void*);

int main(void)
{
   setup();
   run();
}

PWMServo servo;

void setup()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, LOW);

    servo.attach(SERVO_PIN, MIN_ANGLE, MAX_ANGLE);
    servo.write(START_LIDAR_ANGLE);

    xTaskCreate(
            move,                     // task function
            (const portCHAR *)"Move", // debug name
            DRIVE_STACK_SIZE,         // stack size
            nullptr,                  // parameters
            DRIVE_TASK_PRIORITY,      // priority
            nullptr);                 // TaskHandle_treturn value

    xTaskCreate(
            lidar,                     // task function
            (const portCHAR *)"Lidar", // debug name
            LIDAR_STACK_SIZE,          // stack size
            nullptr,                   // parameters
            LIDAR_TASK_PRIORITY,       // priority
            nullptr);                  // TaskHandle_t return value
}

void run()
{
    vTaskStartScheduler();
}

void move(void*)
{
    for (;;) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);  //go forward

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);   //stop

        vTaskDelay(500 / portTICK_PERIOD_MS);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);   //go back

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);  //stop

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void lidar(void*)
{
    int angle = START_LIDAR_ANGLE;
    int direction = 1;

    servo.write(angle);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    for (;;)
    {
        angle += LIDAR_ANGLE_INC * direction;
        if (angle < 0 || angle > 180)
        {
            direction *= -1;
        }

        servo.write(angle);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
