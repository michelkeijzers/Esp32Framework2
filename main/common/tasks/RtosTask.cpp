#include "RtosTask.hpp"

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
    return ESP_OK;
}   
