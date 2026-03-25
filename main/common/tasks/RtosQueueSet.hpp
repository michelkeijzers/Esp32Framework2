#pragma once

#include <cstdint>
#include "IRtosQueueSet.hpp"

class RtosQueueSet : public IRtosQueueSet
{
public:
    RtosQueueSet(uint32_t setLength);
    ~RtosQueueSet() override;

    bool createSet() override;
    bool addToSet(QueueSetMemberHandle_t queueOrSemaphore) override;
    QueueSetMemberHandle_t selectFromSet(TickType_t ticksToWait) override;
    QueueSetHandle_t getHandle() const override;

private:
    QueueSetHandle_t queueSetHandle_ = nullptr;
    uint32_t setLength_;
};


