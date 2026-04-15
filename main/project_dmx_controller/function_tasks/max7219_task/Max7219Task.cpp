#include "Max7219Task.hpp"

static constexpr const char *MAX7219_TASK_NAME = "max7219_task";
static constexpr uint32_t MAX7219_TASK_STACK_SIZE = 2048;
static constexpr UBaseType_t MAX7219_TASK_PRIORITY = 4;

Max7219Task::Max7219Task(IFreeRtosFactory &freeRtosFactory)
    : FunctionTask(freeRtosFactory, MAX7219_TASK_NAME, MAX7219_TASK_STACK_SIZE,
                   MAX7219_TASK_PRIORITY) {}

esp_err_t Max7219Task::init() {
    // TODO: initialise SPI bus and MAX7219 chip
    return ESP_OK;
}

esp_err_t Max7219Task::start() { return FunctionTask::start(); }
