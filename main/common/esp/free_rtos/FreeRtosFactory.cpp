#include "FreeRtosFactory.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

BaseType_t FreeRtosFactory::createTask(
    TaskFunction_t  taskFunction,
    const char     *name,
    uint32_t        stackDepth,
    void           *params,
    UBaseType_t     priority,
    TaskHandle_t   *handle)
{
    return xTaskCreate(taskFunction, name, stackDepth, params, priority, handle);
}

TickType_t FreeRtosFactory::getTickCount()
{
    return xTaskGetTickCount();
}
