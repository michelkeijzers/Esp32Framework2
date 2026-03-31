#pragma once

#include "../esp/free_rtos/IFreeRtosFactory.hpp"
#include "IRtosTask.hpp"
#include "../esp/esp_error/esp_error_if.hpp"

#include <cstdint>

#include <vector>
#include <memory>
#include "RtosQueue.hpp"
#include "IRtosQueueSet.hpp"

class RtosTask : public IRtosTask
{
  public:
    RtosTask(IFreeRtosFactory &freeRtosFactory, const char *name, uint32_t stackSize, UBaseType_t priority);
    virtual ~RtosTask() = default;

    virtual esp_err_t init();
    virtual esp_err_t start();

    // Create and add a queue to the task
    IRtosQueue* createQueue(size_t itemSize, size_t length) override;

  protected:
    // Create and add a queue set to the task.
    IRtosQueueSet* createQueueSet(UBaseType_t eventQueueLength);

    IFreeRtosFactory &freeRtosFactory_;
    const char *name_;
    uint32_t stackSize_;
    UBaseType_t priority_;

    // Store owned queues and queue sets.
    std::vector<std::unique_ptr<RtosQueue>> queues_;
    std::vector<std::unique_ptr<IRtosQueueSet>> queueSets_;
};
