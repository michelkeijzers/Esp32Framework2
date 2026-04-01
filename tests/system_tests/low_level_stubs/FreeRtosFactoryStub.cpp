#include "common/esp/free_rtos/FreeRtosFactory.hpp"
#include "LowLevelStubCounters.hpp"
#include <iostream>

BaseType_t FreeRtosFactory::createTask(
    TaskFunction_t,
    const char *name,
    uint32_t,
    void *,
    UBaseType_t,
    TaskHandle_t *)
{
    ++low_level_stubs::counters().free_rtos_create_task_calls;
    // ...existing code...
    return pdPASS;
}
