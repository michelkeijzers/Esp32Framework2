#include "GpioLedTask.hpp"

static constexpr const char *GPIO_LED_TASK_NAME       = "gpio_led_task";
static constexpr uint32_t    GPIO_LED_TASK_STACK_SIZE  = 2048;
static constexpr UBaseType_t GPIO_LED_TASK_PRIORITY    = 4;

GpioLedTask::GpioLedTask(int gpioPin)
    : SubTask(GPIO_LED_TASK_NAME, GPIO_LED_TASK_STACK_SIZE, GPIO_LED_TASK_PRIORITY)
    , gpioPin_(gpioPin)
{
}

esp_err_t GpioLedTask::init()
{
    // TODO: configure GPIO pin as output
    return ESP_OK;
}

esp_err_t GpioLedTask::start()
{
    // TODO: create FreeRTOS task that drives LED based on state
    return ESP_OK;
}
