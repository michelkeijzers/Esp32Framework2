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
    return RtosTask::init();
}

esp_err_t LoggingTask::start()
{
    return RtosTask::start();
}

void LoggingTask::taskEntry()
{
    if (queues_.empty())
    {
        return;
    }
    IRtosQueue *queue = queues_.front().get();
    int item = 0; // Default to int, since RtosTask::start creates an int queue
    while (true)
    {
        if (queue && queue->receive(&item, portMAX_DELAY))
        {
            // TODO: handle the received item
        }
    }
}
