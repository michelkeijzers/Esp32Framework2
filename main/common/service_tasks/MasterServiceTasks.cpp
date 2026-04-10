#include "MasterServiceTasks.hpp"

#include "../node/NodeStaticInfo.hpp"

namespace
{
    constexpr uint8_t STATUS_TASK_ID = 11; // See ADR_003_Task_IDs
}

MasterServiceTasks::MasterServiceTasks(IFreeRtosFactory &freeRtosFactory, NodeStaticInfo &nodeStaticInfo, int nodeOffset)
    : serviceTasks_(freeRtosFactory, nodeStaticInfo, nodeOffset), statusTask_(freeRtosFactory)
{
    nodeStaticInfo.addTaskStaticInfo(TaskStaticInfo(nodeOffset + STATUS_TASK_ID, "Status Task"));
}

esp_err_t MasterServiceTasks::init()
{
    esp_err_t ret = serviceTasks_.init();
    if (ret != ESP_OK)
        return ret;

    return statusTask_.init();
}

esp_err_t MasterServiceTasks::start()
{
    esp_err_t ret = serviceTasks_.start();
    if (ret != ESP_OK)
        return ret;

    return statusTask_.start();
}

ILoggingTask &MasterServiceTasks::getLoggingTask()
{
    return serviceTasks_.getLoggingTask();
}

IOtaTask &MasterServiceTasks::getOtaTask()
{
    return serviceTasks_.getOtaTask();
}

IStatusTask &MasterServiceTasks::getStatusTask()
{
    return statusTask_;
}
