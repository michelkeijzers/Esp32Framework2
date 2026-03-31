#include "RtosQueueSet.hpp"

RtosQueueSet::RtosQueueSet(uint32_t setLength)
    : IRtosQueueSet(setLength), setLength_(setLength)
{
    queueSetHandle_ = xQueueCreateSet(setLength_);
}

RtosQueueSet::~RtosQueueSet()
{
    if (queueSetHandle_) {
        vQueueDelete(queueSetHandle_);
        queueSetHandle_ = nullptr;
    }
}

bool RtosQueueSet::createSet()
{
    //TODO
     return queueSetHandle_ != nullptr;
 }

bool RtosQueueSet::addToSet(QueueSetMemberHandle_t queueOrSemaphore)
{
    if (!queueSetHandle_ || !queueOrSemaphore) {
        return false;
    }

    return ::xQueueAddToSet(queueOrSemaphore, queueSetHandle_) == pdPASS;
}

QueueSetMemberHandle_t RtosQueueSet::selectFromSet(TickType_t ticksToWait)
{
    if (!queueSetHandle_) {
        return nullptr;
    }

    return ::xQueueSelectFromSet(queueSetHandle_, ticksToWait);
}

QueueSetHandle_t RtosQueueSet::getHandle() const
{
    return queueSetHandle_;
}
