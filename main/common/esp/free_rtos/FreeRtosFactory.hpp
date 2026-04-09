#pragma once

#include "IFreeRtosFactory.hpp"

/**
 * Production implementation of IFreeRtosFactory.
 * Delegates task creation to the FreeRTOS xTaskCreate API.
 * Only compiled for the ESP target; not linked into host test builds.
 */
class FreeRtosFactory : public IFreeRtosFactory
{
public:
    BaseType_t createTask(
        TaskFunction_t  taskFunction,
        const char     *name,
        uint32_t        stackDepth,
        void           *params,
        UBaseType_t     priority,
        TaskHandle_t   *handle) override;

    TickType_t getTickCount();
};
