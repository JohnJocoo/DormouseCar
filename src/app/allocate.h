#pragma once

/**
 * @file
 * @copyright (C) 2020 Anton Frolov johnjocoo@gmail.com
 */

#include "FreeRTOS/portable.h"


void* os_raw_alloc(const unsigned int bytes)
{
    return pvPortMalloc(bytes);
}

void os_raw_dealloc(void* mem)
{
    if (mem == nullptr)
    {
        return;
    }

    vPortFree(mem);
}

template <typename T>
T* os_alloc_n(const unsigned int number)
{
    void* mem = pvPortMalloc(number * sizeof(T) + sizeof(unsigned int));

    if (mem == nullptr)
    {
        return nullptr;
    }

    (*((unsigned int*)mem)) = number;
    mem = (void*)(((unsigned int*)mem) + 1);

    for (unsigned int i = 0; i < number; ++i)
    {
        new (((T*)mem) + i) T();
    }

    return (T*)mem;
}

template <typename T, typename... Args>
T* os_alloc(Args... args)
{
    void* mem = pvPortMalloc(sizeof(T));

    if (mem == nullptr)
    {
        return nullptr;
    }

    new ((T*)mem) T(args...);

    return (T*)mem;
}

template <typename T>
void os_dealloc_n(T* objects)
{
    if (objects == nullptr)
    {
        return;
    }

    const unsigned int number = *(((unsigned int*)objects) - 1);

    for (unsigned int i = number; i > 0; --i)
    {
        (objects + (i - 1))->~T();
    }

    vPortFree((((unsigned int*)objects) - 1));
}

template <typename T>
void os_dealloc(T* object)
{
    if (object == nullptr)
    {
        return;
    }

    object->~T();

    vPortFree(object);
}
