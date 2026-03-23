#include "WebserverTask.hpp"

static constexpr const char *WEBSERVER_TASK_NAME = "webserver_task";
static constexpr uint32_t WEBSERVER_TASK_STACK_SIZE = 8192;
static constexpr UBaseType_t WEBSERVER_TASK_PRIORITY = 6;

WebserverTask::WebserverTask(IApiServer &apiServer)
    : RtosTask(WEBSERVER_TASK_NAME, WEBSERVER_TASK_STACK_SIZE, WEBSERVER_TASK_PRIORITY), apiServer_(apiServer)
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
