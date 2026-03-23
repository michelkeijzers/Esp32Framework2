#include "MasterTask.hpp"

static constexpr const char *MASTER_TASK_NAME       = "master_task";
static constexpr uint32_t    MASTER_TASK_STACK_SIZE  = 8192;
static constexpr UBaseType_t MASTER_TASK_PRIORITY    = 7;

MasterTask::MasterTask(IMasterBridge &masterBridge)
    : RtosTask(MASTER_TASK_NAME, MASTER_TASK_STACK_SIZE, MASTER_TASK_PRIORITY)
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
