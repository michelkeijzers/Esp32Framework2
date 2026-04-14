#include "FunctionTask.hpp"

FunctionTask::FunctionTask(IFreeRtosFactory &freeRtosFactory, const char *name, uint32_t stackSize,
                           UBaseType_t priority)
    : RtosTask(freeRtosFactory, name, stackSize, priority) {}