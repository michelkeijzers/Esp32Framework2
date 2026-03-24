#pragma once

#include "free_rtos_if.hpp"
#include <cstdint>

/**
 * Abstracts FreeRTOS task creation to allow test builds to inject a mock
 * implementation instead of calling xTaskCreate directly.
 */
class IFreeRtosFactory
{
public:
    virtual ~IFreeRtosFactory() = default;

    virtual BaseType_t createTask(
        TaskFunction_t  taskFunction,
        const char     *name,
        uint32_t        stackDepth,
        void           *params,
        UBaseType_t     priority,
        TaskHandle_t   *handle) = 0;
};
