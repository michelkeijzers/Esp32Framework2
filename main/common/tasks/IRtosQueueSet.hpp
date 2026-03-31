#pragma once

#include <cstdint>
#include "../esp/free_rtos/free_rtos_if.hpp"

class IRtosQueueSet {
public:
    IRtosQueueSet(uint32_t setLength);
    virtual ~IRtosQueueSet();

    virtual bool createSet() = 0;
    virtual bool addToSet(QueueSetMemberHandle_t queueOrSemaphore) = 0;
    virtual QueueSetMemberHandle_t selectFromSet(TickType_t ticksToWait) = 0;
    virtual QueueSetHandle_t getHandle() const = 0;
};
