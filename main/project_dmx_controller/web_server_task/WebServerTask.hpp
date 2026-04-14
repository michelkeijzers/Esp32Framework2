#pragma once

#include "../../../common/tasks/RtosTask.hpp"
#include "IApiServer.hpp"
#include "IWebServerTask.hpp"

/**
 * Project web server task wrapper around a project API server.
 */
class WebServerTask : public RtosTask, public IWebServerTask {
   public:
    WebServerTask(IFreeRtosFactory &freeRtosFactory, IApiServer &apiServer, const char *taskName,
                  uint32_t stackSize, UBaseType_t taskPriority);
    ~WebServerTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;
    esp_err_t stop() override;

   private:
    void taskEntry() override;

    IApiServer &apiServer_;
};
