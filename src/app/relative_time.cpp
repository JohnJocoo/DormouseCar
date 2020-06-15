/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

#include "relative_time.h"

#include "FreeRTOS/task.h"

Duration Duration::timeFromEpoch()
{
    return Duration{xTaskGetTickCount() * portTICK_PERIOD_MS};
}

DurationBaseT Duration::toOSTicks() const
{
    return m_millis / portTICK_PERIOD_MS;
}
