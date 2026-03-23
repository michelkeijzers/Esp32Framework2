#pragma once

#include "ISlaveTask.hpp"
#include "../../common/tasks/RtosTask.hpp"
#include "../../common/bridge/slave_bridge/ISlaveBridge.hpp"

/**
 * Concrete slave task.
 *
 * Runs as a FreeRTOS task and handles incoming commands from the master
 * node (received via SlaveBridge), dispatching them to the appropriate
 * sub tasks.
 */
class SlaveTask : public RtosTask, public ISlaveTask
{
public:
    explicit SlaveTask(ISlaveBridge &slaveBridge);
    ~SlaveTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    ISlaveBridge &slaveBridge_;
};
