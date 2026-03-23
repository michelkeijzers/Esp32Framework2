#pragma once

#include "IWebserverTask.hpp"
#include "IApiServer.hpp"
#include "../tasks/RtosTask.hpp"

/**
 * Concrete webserver task.
 *
 * Inherits from RtosTask to run as a FreeRTOS task and owns (by reference)
 * an IApiServer instance that registers the application REST API endpoints.
 */
class WebserverTask : public RtosTask, public IWebserverTask
{
public:
    explicit WebserverTask(IApiServer &apiServer);
    ~WebserverTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;
    esp_err_t stop()  override;

private:
    IApiServer &apiServer_;
};
