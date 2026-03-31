#include "common/esp/free_rtos/FreeRtosFactory.hpp"
#include "LowLevelStubCounters.hpp"

BaseType_t FreeRtosFactory::createTask(
    TaskFunction_t,
    const char *,
    uint32_t,
    void *,
    UBaseType_t,
    TaskHandle_t *)
{
    ++low_level_stubs::counters().free_rtos_create_task_calls;
    return pdPASS;
}
