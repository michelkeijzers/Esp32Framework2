#include "RtosQueue.hpp"
#include "RtosTask.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

RtosTask::RtosTask(const char *name, uint32_t stackSize, UBaseType_t priority)
    : name_(name), stackSize_(stackSize), priority_(priority)
{
}


esp_err_t RtosTask::init()
{
    return ESP_OK;
}


esp_err_t RtosTask::start()
{
    if (xTaskCreate(
            [](void *arg) {
                auto task = static_cast<RtosTask *>(arg);
                task->start();
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

IRtosQueue* RtosTask::createQueue(size_t itemSize, size_t length) {
    auto queue = std::make_unique<RtosQueue>(itemSize, length);
    IRtosQueue* queuePtr = queue.get();
    queues_.push_back(std::move(queue));
    return queuePtr;
}
