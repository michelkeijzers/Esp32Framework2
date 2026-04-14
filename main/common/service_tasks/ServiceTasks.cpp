#include "ServiceTasks.hpp"

#include "../node/NodeStaticInfo.hpp"

namespace {
constexpr uint8_t LOGGING_TASK_ID = 10;  // See ADR_003_Task_IDs
constexpr uint8_t OTA_TASK_ID = 12;      // See ADR_003_Task_IDs
}  // namespace

ServiceTasks::ServiceTasks(IFreeRtosFactory& freeRtosFactory, NodeStaticInfo& nodeStaticInfo,
                           int nodeOffset)
    : loggingTask_(freeRtosFactory), otaTask_(freeRtosFactory) {
    nodeStaticInfo.addTaskStaticInfo(TaskStaticInfo(nodeOffset + LOGGING_TASK_ID, "Logging Task"));
    nodeStaticInfo.addTaskStaticInfo(TaskStaticInfo(nodeOffset + OTA_TASK_ID, "OTA Task"));
}

esp_err_t ServiceTasks::init() {
    esp_err_t ret = loggingTask_.init();
    if (ret != ESP_OK) {
        return ret;
    }

    ret = otaTask_.init();
    return ret;
}

esp_err_t ServiceTasks::start() {
    esp_err_t ret = loggingTask_.start();
    if (ret != ESP_OK) {
        return ret;
    }

    ret = otaTask_.start();
    return ret;
}

ILoggingTask& ServiceTasks::getLoggingTask() { return loggingTask_; }

IOtaTask& ServiceTasks::getOtaTask() { return otaTask_; }
