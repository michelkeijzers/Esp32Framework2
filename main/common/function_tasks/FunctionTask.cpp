#include "FunctionTask.hpp"

FunctionTask::FunctionTask(IFreeRtosFactory &freeRtosFactory, const char *name, uint32_t stackSize,
                           UBaseType_t priority)
    : RtosTask(freeRtosFactory, name, stackSize, priority) {}

esp_err_t FunctionTask::start() { return RtosTask::start(); }