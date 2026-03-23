#pragma once

#include "IButtonTask.hpp"
#include "../../../common/sub_tasks/SubTask.hpp"

/**
 * Concrete button input sub task.
 *
 * Monitors a GPIO pin connected to a button and fires callbacks on
 * press/release events.
 */
class ButtonTask : public SubTask, public IButtonTask
{
public:
    explicit ButtonTask(int gpioPin);
    ~ButtonTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    int gpioPin_;
};
