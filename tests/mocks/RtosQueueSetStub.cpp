#include "common/tasks/RtosQueueSet.hpp"

RtosQueueSet::~RtosQueueSet() {}

bool RtosQueueSet::createSet()
{
    queueSetHandle_ = nullptr;
    return true;
}

bool RtosQueueSet::addToSet(QueueSetMemberHandle_t)
{
    return true;
}

QueueSetMemberHandle_t RtosQueueSet::selectFromSet(TickType_t)
{
    return nullptr;
}

QueueSetHandle_t RtosQueueSet::getHandle() const
{
    return queueSetHandle_;
}
