#pragma once

#include "../../../common/function_tasks/FunctionTask.hpp"
#include "IButtonTask.hpp"

/**
 * Concrete button input function task.
 *
 * Monitors a GPIO pin connected to a button and fires callbacks on
 * press/release events.
 */
class ButtonTask : public FunctionTask, public IButtonTask {
   public:
    ButtonTask(IFreeRtosFactory &freeRtosFactory, int gpioPin);
    ~ButtonTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

   private:
    int gpioPin_;
};
