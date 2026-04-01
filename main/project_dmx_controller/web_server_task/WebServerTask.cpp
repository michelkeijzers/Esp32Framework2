#include "WebServerTask.hpp"

WebServerTask::WebServerTask(IFreeRtosFactory &freeRtosFactory, IApiServer &apiServer,
                             const char *taskName, uint32_t stackSize, UBaseType_t taskPriority)
    : RtosTask(freeRtosFactory, taskName, stackSize, taskPriority), apiServer_(apiServer)
{
}

esp_err_t WebServerTask::init()
{
    return ESP_OK;
}

esp_err_t WebServerTask::start()
{
    esp_err_t err = RtosTask::start();
    if (err != ESP_OK)
    {
        return err;
    }

    apiServer_.start();
    return ESP_OK;
}

esp_err_t WebServerTask::stop()
{
    apiServer_.stop();
    return ESP_OK;
}
