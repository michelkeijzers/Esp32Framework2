#include "StatusTask.hpp"

static constexpr const char *STATUS_TASK_NAME       = "status_task";
static constexpr uint32_t    STATUS_TASK_STACK_SIZE  = 4096;
static constexpr UBaseType_t STATUS_TASK_PRIORITY    = 5;

StatusTask::StatusTask(IFreeRtosFactory &freeRtosFactory)
    : RtosTask(freeRtosFactory, STATUS_TASK_NAME, STATUS_TASK_STACK_SIZE, STATUS_TASK_PRIORITY)
{
    // Queue will be created in init
}

esp_err_t StatusTask::init()
{
    // Optionally, create a queue with the correct size if not already created in RtosTask::start
    // If you want a specific size/type, you can call createQueue here
    return RtosTask::init();
}

esp_err_t StatusTask::start()
{
    return RtosTask::start();
}

void StatusTask::taskEntry()
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
