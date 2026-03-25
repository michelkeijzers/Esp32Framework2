#include "MasterTask.hpp"

MasterTask::MasterTask(IFreeRtosFactory &freeRtosFactory, IMasterBridge &masterBridge,
                       const char *taskName, uint32_t stackSize, UBaseType_t taskPriority)
    : RtosTask(freeRtosFactory, taskName, stackSize, taskPriority)
    , masterBridge_(masterBridge)
{
}

esp_err_t MasterTask::init()
{
    // Example: adjust item size and length as needed
    constexpr size_t webServerItemSize = 64;
    constexpr size_t webServerQueueLen = 8;
    constexpr size_t masterBridgeItemSize = 64;
    constexpr size_t masterBridgeQueueLen = 8;

    webServerQueue_ = createQueue(webServerItemSize, webServerQueueLen);
    if (!webServerQueue_) {
        return ESP_FAIL;
    }
    
    masterBridgeQueue_ = createQueue(masterBridgeItemSize, masterBridgeQueueLen);
    if (!masterBridgeQueue_) {
        return ESP_FAIL;
    }

    if (!webServerQueue_ || !masterBridgeQueue_) {
        return ESP_FAIL;
    }

    return masterBridge_.init();
}

esp_err_t MasterTask::start()
{
    esp_err_t err = RtosTask::start();
    if (err != ESP_OK) return err;



    return ESP_OK;
}
