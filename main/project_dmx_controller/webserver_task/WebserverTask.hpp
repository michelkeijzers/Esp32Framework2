#pragma once

#include "IWebserverTask.hpp"
#include "IApiServer.hpp"
#include "../../../common/tasks/RtosTask.hpp"

/**
 * Project webserver task wrapper around a project API server.
 */
class WebserverTask : public RtosTask, public IWebserverTask
{
public:
    WebserverTask(IApiServer &apiServer, const char *taskName, uint32_t stackSize, UBaseType_t taskPriority);
    ~WebserverTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;
    esp_err_t stop() override;

private:
    IApiServer &apiServer_;
};
