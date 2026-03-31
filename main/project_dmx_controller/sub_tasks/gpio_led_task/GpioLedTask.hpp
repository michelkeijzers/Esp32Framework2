#pragma once

#include "IGpioLedTask.hpp"
#include "../../../common/sub_tasks/SubTask.hpp"

/**
 * Concrete GPIO LED output sub task.
 *
 * Controls an LED (or simple GPIO output) connected to a GPIO pin.
 */
class GpioLedTask : public SubTask, public IGpioLedTask
{
public:
    GpioLedTask(IFreeRtosFactory &freeRtosFactory, int gpioPin);
    ~GpioLedTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    int gpioPin_;
};
