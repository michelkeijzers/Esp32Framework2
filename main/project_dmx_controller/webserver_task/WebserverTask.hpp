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
    explicit WebserverTask(IApiServer &apiServer);
    ~WebserverTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;
    esp_err_t stop() override;

private:
    IApiServer &apiServer_;
};
