#pragma once

/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

#include <stdint.h>


#define _DURATION_BASE_T_MAX UINT32_MAX


typedef uint32_t DurationBaseT;

class Duration
{
public:
    // can not be used from ISR
    static Duration timeFromEpoch();
    static Duration epoch();
    static Duration invalid();
    static Duration fromMillis(DurationBaseT millis);
    static Duration fromSeconds(DurationBaseT seconds,
                                DurationBaseT millis = 0);
    static Duration fromMinutes(DurationBaseT minutes,
                                DurationBaseT seconds = 0,
                                DurationBaseT millis = 0);

    Duration(const Duration& other);
    Duration& operator=(const Duration& other);
    ~Duration();

    bool isInvalid() const;

    DurationBaseT millisTotal() const;
    DurationBaseT secondsTotal() const;
    DurationBaseT minutesTotal() const;

    DurationBaseT millisInSecond() const;
    DurationBaseT secondsInMinute() const;

    DurationBaseT toOSTicks() const;

private:
    explicit Duration(DurationBaseT millis);

    DurationBaseT m_millis;

}; // class Duration

bool operator==(const Duration& val1, const Duration& val2)
{
    return val1.m_millis == val2.m_millis;
}

bool operator!=(const Duration& val1, const Duration& val2)
{
    return val1.m_millis != val2.m_millis;
}

bool operator>(const Duration& val1, const Duration& val2)
{
    return val1.m_millis > val2.m_millis;
}

bool operator<(const Duration& val1, const Duration& val2)
{
    return val1.m_millis < val2.m_millis;
}

bool operator>=(const Duration& val1, const Duration& val2)
{
    return val1.m_millis >= val2.m_millis;
}

bool operator<=(const Duration& val1, const Duration& val2)
{
    return val1.m_millis <= val2.m_millis;
}

Duration operator+(const Duration& val1, const Duration& val2)
{
    return Duration{val1.m_millis + val2.m_millis};
}

Duration operator-(const Duration& val1, const Duration& val2)
{
    return Duration{val1.m_millis - val2.m_millis};
}

Duration& operator+=(Duration& val1, const Duration& val2)
{
    val1.m_millis += val2.m_millis;
    return val1;
}

Duration& operator-=(Duration& val1, const Duration& val2)
{
    val1.m_millis -= val2.m_millis;
    return val1;
}

Duration operator*(const Duration& val1, unsigned int val2)
{
    return Duration{val1.m_millis * val2};
}

Duration operator*(const Duration& val1, float val2)
{
    return Duration{static_cast<DurationBaseT>(val1.m_millis * val2)};
}

Duration& operator*=(Duration& val1, unsigned int val2)
{
    val1.m_millis *= val2;
    return val1;
}

Duration& operator*=(Duration& val1, float val2)
{
    val1.m_millis = static_cast<DurationBaseT>(val1.m_millis * val2);
    return val1;
}

float operator/(const Duration& val1, const Duration& val2)
{
    return val1.m_millis / (float)val2.m_millis;
}

Duration operator/(const Duration& val1, float val2)
{
    return Duration{static_cast<DurationBaseT>(val1.m_millis / val2)};
}

Duration& operator/=(Duration& val1, float val2)
{
    val1.m_millis = static_cast<DurationBaseT>(val1.m_millis / val2);
    return val1;
}

Duration operator%(const Duration& val1, const Duration& val2)
{
    return Duration{val1.m_millis % val2.m_millis};
}

Duration& operator%=(Duration& val1, const Duration& val2)
{
    val1.m_millis = val1.m_millis % val2.m_millis;
    return val1;
}

Duration Duration::epoch()
{
    return Duration{0};
}

Duration Duration::invalid()
{
    return Duration{_DURATION_BASE_T_MAX};
}

Duration Duration::fromMillis(DurationBaseT millis)
{
    return Duration{millis};
}

Duration Duration::fromSeconds(DurationBaseT seconds, DurationBaseT millis)
{
    return Duration{seconds*1000 + millis};
}

Duration Duration::fromMinutes(DurationBaseT minutes, DurationBaseT seconds,
                               DurationBaseT millis)
{
    return Duration{minutes*60000 + seconds*1000 + millis};
}

Duration::Duration(const Duration& other)
: m_millis{other.m_millis}
{

}

Duration& Duration::operator=(const Duration& other)
{
    m_millis = other.m_millis;
    return *this;
}

Duration::~Duration() = default;

bool Duration::isInvalid() const
{
    return m_millis == _DURATION_BASE_T_MAX;
}

DurationBaseT Duration::millisTotal() const
{
    return m_millis;
}

DurationBaseT Duration::secondsTotal() const
{
    return m_millis / 1000;
}

DurationBaseT Duration::minutesTotal() const
{
    return m_millis / 60000;
}

DurationBaseT Duration::millisInSecond() const
{
    return m_millis % 1000;
}

DurationBaseT Duration::secondsInMinute() const
{
    return secondsTotal() % 60;
}

Duration::Duration(DurationBaseT millis)
: m_millis{millis}
{

}
