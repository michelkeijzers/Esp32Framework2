#include "RtosQueue.hpp"
#include "RtosQueueSet.hpp"
#include "RtosTask.hpp"
#include <iostream>

RtosTask::RtosTask(IFreeRtosFactory &freeRtosFactory, const char *name, uint32_t stackSize, UBaseType_t priority)
    : freeRtosFactory_(freeRtosFactory), name_(name), stackSize_(stackSize), priority_(priority)
{
}


esp_err_t RtosTask::init()
{
    return ESP_OK;
}


esp_err_t RtosTask::start()
{
    if (freeRtosFactory_.createTask(
            [](void *arg) {
                auto task = static_cast<RtosTask *>(arg);
                task->taskEntry();
            },
            name_,
            stackSize_ / sizeof(StackType_t),
            this,
            priority_,
            nullptr) != pdPASS)
    {
        return ESP_FAIL;
    }

    createQueue(sizeof(int), 10); //TODO
    return ESP_OK;
}

void RtosTask::taskEntry()
{
}

IRtosQueue* RtosTask::createQueue(size_t itemSize, size_t length) {
    auto queue = std::make_unique<RtosQueue>(itemSize, length);
    if (!queue->create()) {
        return nullptr;
    }

    IRtosQueue* queuePtr = queue.get();
    queues_.push_back(std::move(queue));
    return queuePtr;
}

// IRtosQueueSet* RtosTask::createQueueSet(UBaseType_t eventQueueLength)
// {
//     auto queueSet = std::make_unique<RtosQueueSet>(eventQueueLength);
//     if (!queueSet->createSet()) {
//         return nullptr;
//     }

//     IRtosQueueSet* queueSetPtr = queueSet.get();
//     queueSets_.push_back(std::move(queueSet));
//     return queueSetPtr;
// }
