#pragma once

#include "ISubTask.hpp"
#include "../tasks/RtosTask.hpp"

#include <cstdint>

/**
 * Abstract base class for all sub tasks.
 *
 * Inherits from both RtosTask (lifecycle, FreeRTOS integration) and
 * ISubTask (sub task contract for dependency injection).
 *
 * Concrete sub tasks must implement init() and start() which satisfy the
 * pure virtual requirements from both base classes.
 */
class SubTask : public RtosTask, public ISubTask
{
public:
    SubTask(IFreeRtosFactory &freeRtosFactory, const char *name, uint32_t stackSize, UBaseType_t priority);
    virtual ~SubTask() = default;

    virtual esp_err_t init()  = 0;
    virtual esp_err_t start() = 0;
};
