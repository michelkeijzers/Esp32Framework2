#include "LoggingTask.hpp"

static constexpr const char *LOGGING_TASK_NAME       = "logging_task";
static constexpr uint32_t    LOGGING_TASK_STACK_SIZE  = 4096;
static constexpr UBaseType_t LOGGING_TASK_PRIORITY    = 5;

LoggingTask::LoggingTask(IFreeRtosFactory &freeRtosFactory)
    : RtosTask(freeRtosFactory, LOGGING_TASK_NAME, LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY)
{
}

esp_err_t LoggingTask::init()
{
    // TODO: initialise log ring buffer and SSE client list
    return ESP_OK;
}

esp_err_t LoggingTask::start()
{
    // TODO: create FreeRTOS task that reads the ring buffer and forwards
    //       entries to all registered SSE clients
    return ESP_OK;
}
