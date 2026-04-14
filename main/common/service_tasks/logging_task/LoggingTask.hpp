#pragma once

#include "../../tasks/RtosTask.hpp"
#include "ILoggingTask.hpp"

/**
 * Concrete logging task.
 *
 * Runs as a FreeRTOS task and streams log output to registered
 * Server-Sent Event (SSE) clients via the /api/v1/logging endpoint.
 */
class LoggingTask : public RtosTask, public ILoggingTask {
   public:
    explicit LoggingTask(IFreeRtosFactory &freeRtosFactory);
    ~LoggingTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

   protected:
    void taskEntry() override;
};
