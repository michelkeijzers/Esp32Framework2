#include "StatusTask.hpp"

static constexpr const char *STATUS_TASK_NAME       = "status_task";
static constexpr uint32_t    STATUS_TASK_STACK_SIZE  = 4096;
static constexpr UBaseType_t STATUS_TASK_PRIORITY    = 5;

StatusTask::StatusTask()
    : RtosTask(STATUS_TASK_NAME, STATUS_TASK_STACK_SIZE, STATUS_TASK_PRIORITY)
{
}

esp_err_t StatusTask::init()
{
    // TODO: initialise SSE client list and node status data structures
    return ESP_OK;
}

esp_err_t StatusTask::start()
{
    // TODO: create FreeRTOS task that periodically collects node status
    //       and pushes updates to all registered SSE clients
    return ESP_OK;
}
