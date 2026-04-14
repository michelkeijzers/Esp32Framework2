#pragma once

#include "../../common/bridge/slave_bridge/ISlaveBridge.hpp"
#include "../../common/tasks/RtosTask.hpp"
#include "ISlaveTask.hpp"

/**
 * Concrete slave task.
 *
 * Runs as a FreeRTOS task and handles incoming commands from the master
 * node (received via SlaveBridge), dispatching them to the appropriate
 * function tasks.
 */
class SlaveTask : public RtosTask, public ISlaveTask {
   public:
    SlaveTask(IFreeRtosFactory &freeRtosFactory, ISlaveBridge &slaveBridge);
    ~SlaveTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

   private:
    ISlaveBridge &slaveBridge_;
};
