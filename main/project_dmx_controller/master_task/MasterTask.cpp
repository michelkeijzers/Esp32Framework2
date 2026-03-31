#include "MasterTask.hpp"
#include "../../common/bridge/master_bridge/IMasterBridge.hpp"

constexpr size_t webServerItemSize = 64;
constexpr size_t webServerQueueLen = 8;
constexpr size_t masterBridgeItemSize = 64;
constexpr size_t masterBridgeQueueLen = 8;

MasterTask::MasterTask(IFreeRtosFactory &freeRtosFactory, IMasterBridge &masterBridge, IRtosQueue &masterBridgeQueue, IWebServerTask &webServerTask,
                       IRtosQueue &webServerQueue, IRtosQueueSet &queueSet,
                       const char *taskName, uint32_t stackSize, UBaseType_t taskPriority)
    : RtosTask(freeRtosFactory, taskName, stackSize, taskPriority), masterBridge_(masterBridge), masterBridgeQueue_(masterBridgeQueue), webServerTask(webServerTask), webServerQueue_(webServerQueue), queueSet_(queueSet)
{
}

esp_err_t MasterTask::init()
{
    // Example: adjust item size and length as needed


    if (!webServerQueue_.create() != ESP_OK) 
    {
        return ESP_FAIL;
    }
  

    if (masterBridge_.init() != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}


esp_err_t MasterTask::start()
{
    esp_err_t err = RtosTask::start();
    if (err != ESP_OK) {
        return err;
    }

    if (!queueSet_.addToSet(webServerQueue_.getHandle())) {
        return ESP_FAIL;
    }

    if (!queueSet_.addToSet(masterBridgeQueue_.getHandle())) {
        return ESP_FAIL;
    }

#ifdef ESP_PLATFORM
    while (true)
    {
        const QueueSetMemberHandle_t activated = queueSet_.selectFromSet(portMAX_DELAY);
        if (activated == webServerQueue_.getHandle()) {
            uint8_t item[64];
            webServerQueue_.receive(item, 0);
        } else if (activated == masterBridgeQueue_.getHandle()) {
            uint8_t item[64];
            masterBridgeQueue_.receive(item, 0);
        }
    }
#endif

    return ESP_OK;
}
