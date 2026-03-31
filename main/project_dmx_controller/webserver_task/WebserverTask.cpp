#include "WebserverTask.hpp"

WebserverTask::WebserverTask(IFreeRtosFactory &freeRtosFactory, IApiServer &apiServer,
                             const char *taskName, uint32_t stackSize, UBaseType_t taskPriority)
    : RtosTask(freeRtosFactory, taskName, stackSize, taskPriority)
    , apiServer_(apiServer)
{
}

esp_err_t WebserverTask::init()
{
    return ESP_OK;
}

esp_err_t WebserverTask::start()
{
    apiServer_.start();
    return ESP_OK;
}

esp_err_t WebserverTask::stop()
{
    apiServer_.stop();
    return ESP_OK;
}
