#pragma once

#include "IFunctionTask.hpp"
#include "../tasks/RtosTask.hpp"

#include <cstdint>

/**
 * Abstract base class for all function tasks.
 *
 * Inherits from both RtosTask (lifecycle, FreeRTOS integration) and
 * IFunctionTask (function task contract for dependency injection).
 *
 * Concrete function tasks must implement init() and start() which satisfy the
 * pure virtual requirements from both base classes.
 */
class FunctionTask : public RtosTask, public IFunctionTask
{
public:
    FunctionTask(IFreeRtosFactory &freeRtosFactory, const char *name, uint32_t stackSize, UBaseType_t priority);
    virtual ~FunctionTask() = default;

    virtual esp_err_t init() = 0;
    virtual esp_err_t start() = 0;
};