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
    

    


    // TODO: create FreeRTOS task that handles inter-node communication,
    //       applies preset values to DMX outputs, and processes slave status
    return ESP_OK;
}
