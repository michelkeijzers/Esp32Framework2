#include "MasterTask.hpp"

MasterTask::MasterTask(IMasterBridge &masterBridge, const char *taskName, uint32_t stackSize, UBaseType_t taskPriority)
    : RtosTask(taskName, stackSize, taskPriority)
    , masterBridge_(masterBridge)
{
}

esp_err_t MasterTask::init()
{
    return masterBridge_.init();
}

esp_err_t MasterTask::start()
{
    esp_err_t err = RtosTask::start();
    if (err != ESP_OK) return err;



    return ESP_OK;
}
