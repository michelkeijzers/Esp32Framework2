#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "../esp/esp_error/esp_error_if.hpp"
#include "../esp/free_rtos/IFreeRtosFactory.hpp"
#include "IRtosQueueSet.hpp"
#include "IRtosTask.hpp"
#include "RtosQueue.hpp"

class RtosTask : public IRtosTask {
   public:
    RtosTask(IFreeRtosFactory &freeRtosFactory, const char *name, uint32_t stackSize,
             UBaseType_t priority);
    virtual ~RtosTask() = default;

    esp_err_t init() override;
    esp_err_t start() override;

    // Create and add a queue to the task
    IRtosQueue *createQueue(size_t itemSize, size_t length) override;

    TickType_t getTickCount() override;

   protected:
    virtual void taskEntry();

    // Create and add a queue set to the task.
    IRtosQueueSet *createQueueSet(UBaseType_t eventQueueLength);

    IFreeRtosFactory &freeRtosFactory_;
    const char *name_;
    uint32_t stackSize_;
    UBaseType_t priority_;

    // Store owned queues and queue sets.
    std::vector<std::unique_ptr<RtosQueue>> queues_;
    std::vector<std::unique_ptr<IRtosQueueSet>> queueSets_;
};
