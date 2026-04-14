#include <iostream>

#include "LowLevelStubCounters.hpp"
#include "common/esp/free_rtos/FreeRtosFactory.hpp"

BaseType_t FreeRtosFactory::createTask(TaskFunction_t taskFunction, const char *name, uint32_t,
                                       void *params, UBaseType_t, TaskHandle_t *) {
    ++low_level_stubs::counters().free_rtos_create_task_calls;
    if (taskFunction != nullptr) {
        taskFunction(params);
    }

    return pdPASS;
}

TickType_t FreeRtosFactory::getTickCount() {
    ++low_level_stubs::counters().free_rtos_get_tick_count_calls;
    return 0;
}
