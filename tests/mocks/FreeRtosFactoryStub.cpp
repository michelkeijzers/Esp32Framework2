#include "common/esp/free_rtos/FreeRtosFactory.hpp"

BaseType_t FreeRtosFactory::createTask(TaskFunction_t, const char *, uint32_t, void *,
                                       UBaseType_t, TaskHandle_t *) {
    return pdPASS;
}

TickType_t FreeRtosFactory::getTickCount() { return 0; }
