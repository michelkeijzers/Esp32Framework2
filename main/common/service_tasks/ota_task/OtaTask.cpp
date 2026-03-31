#include "OtaTask.hpp"

static constexpr const char *OTA_TASK_NAME       = "ota_task";
static constexpr uint32_t    OTA_TASK_STACK_SIZE  = 8192;
static constexpr UBaseType_t OTA_TASK_PRIORITY    = 8;

OtaTask::OtaTask(IFreeRtosFactory &freeRtosFactory)
    : RtosTask(freeRtosFactory, OTA_TASK_NAME, OTA_TASK_STACK_SIZE, OTA_TASK_PRIORITY)
{
}

esp_err_t OtaTask::init()
{
    // TODO: initialise OTA partition handles and state machine
    return ESP_OK;
}

esp_err_t OtaTask::start()
{
    // TODO: create FreeRTOS task that processes incoming firmware chunks
    //       and coordinates the flash write sequence
    return ESP_OK;
}
