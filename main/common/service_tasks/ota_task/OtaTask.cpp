#include "OtaTask.hpp"

static constexpr const char *OTA_TASK_NAME       = "ota_task";
static constexpr uint32_t    OTA_TASK_STACK_SIZE  = 8192;
static constexpr UBaseType_t OTA_TASK_PRIORITY    = 8;

OtaTask::OtaTask(IFreeRtosFactory &freeRtosFactory)
    : RtosTask(freeRtosFactory, OTA_TASK_NAME, OTA_TASK_STACK_SIZE, OTA_TASK_PRIORITY)
{
}

esp_err_t OtaTask::init()
{
    // Optionally, create a queue with the correct size if not already created in RtosTask::start
    // If you want a specific size/type, you can call createQueue here
    return RtosTask::init();
}

esp_err_t OtaTask::start()
{
    return RtosTask::start();
}

void OtaTask::taskEntry()
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
