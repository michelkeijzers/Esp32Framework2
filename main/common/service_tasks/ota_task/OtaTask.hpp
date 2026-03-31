#pragma once

#include "IOtaTask.hpp"
#include "../../tasks/RtosTask.hpp"

/**
 * Concrete OTA (Over-The-Air firmware update) task.
 *
 * Runs as a FreeRTOS task and coordinates firmware chunk reception,
 * validation, and flashing for master and slave nodes.
 */
class OtaTask : public RtosTask, public IOtaTask
{
public:
    explicit OtaTask(IFreeRtosFactory &freeRtosFactory);
    ~OtaTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;
};
