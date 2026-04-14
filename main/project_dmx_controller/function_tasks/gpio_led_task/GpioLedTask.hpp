#pragma once

#include "../../../common/function_tasks/FunctionTask.hpp"
#include "IGpioLedTask.hpp"

/**
 * Concrete GPIO LED output function task.
 *
 * Controls an LED (or simple GPIO output) connected to a GPIO pin.
 */
class GpioLedTask : public FunctionTask, public IGpioLedTask {
   public:
    GpioLedTask(IFreeRtosFactory &freeRtosFactory, int gpioPin);
    ~GpioLedTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

   private:
    int gpioPin_;
};
