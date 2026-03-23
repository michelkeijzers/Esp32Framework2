#include "SlaveTask.hpp"

static constexpr const char *SLAVE_TASK_NAME       = "slave_task";
static constexpr uint32_t    SLAVE_TASK_STACK_SIZE  = 8192;
static constexpr UBaseType_t SLAVE_TASK_PRIORITY    = 7;

SlaveTask::SlaveTask(ISlaveBridge &slaveBridge)
    : RtosTask(SLAVE_TASK_NAME, SLAVE_TASK_STACK_SIZE, SLAVE_TASK_PRIORITY)
    , slaveBridge_(slaveBridge)
{
}

esp_err_t SlaveTask::init()
{
    return slaveBridge_.init();
}

esp_err_t SlaveTask::start()
{
    // TODO: create FreeRTOS task that processes commands received by the
    //       SlaveBridge and dispatches them to the appropriate sub tasks
    return ESP_OK;
}
