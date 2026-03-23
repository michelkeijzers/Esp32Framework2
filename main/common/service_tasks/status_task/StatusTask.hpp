#pragma once

#include "IStatusTask.hpp"
#include "../../tasks/RtosTask.hpp"

/**
 * Concrete status task.
 *
 * Runs as a FreeRTOS task and periodically pushes node status updates to
 * registered Server-Sent Event (SSE) clients via the /api/v1/status endpoint.
 */
class StatusTask : public RtosTask, public IStatusTask
{
public:
    StatusTask();
    ~StatusTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;
};
