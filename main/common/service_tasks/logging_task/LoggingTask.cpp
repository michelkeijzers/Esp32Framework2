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
    // Optionally, create a queue with the correct size if not already created in RtosTask::start
    // If you want a specific size/type, you can call createQueue here
    return RtosTask::init();
}

esp_err_t LoggingTask::start()
{
    return RtosTask::start();
}

void LoggingTask::taskEntry()
{
    // Use the first queue from RtosTask (created in start or init)
    if (queues_.empty())
    {
        // No queue to listen to
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
