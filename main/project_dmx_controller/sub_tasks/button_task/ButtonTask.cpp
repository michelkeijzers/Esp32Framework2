#include "ButtonTask.hpp"

static constexpr const char *BUTTON_TASK_NAME       = "button_task";
static constexpr uint32_t    BUTTON_TASK_STACK_SIZE  = 2048;
static constexpr UBaseType_t BUTTON_TASK_PRIORITY    = 5;

ButtonTask::ButtonTask(int gpioPin)
    : SubTask(BUTTON_TASK_NAME, BUTTON_TASK_STACK_SIZE, BUTTON_TASK_PRIORITY)
    , gpioPin_(gpioPin)
{
}

esp_err_t ButtonTask::init()
{
    // TODO: configure GPIO pin as input with pull-up
    return ESP_OK;
}

esp_err_t ButtonTask::start()
{
    // TODO: create FreeRTOS task that polls / uses ISR for button state
    return ESP_OK;
}
