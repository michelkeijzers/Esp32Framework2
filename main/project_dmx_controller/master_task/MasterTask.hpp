#pragma once

#include "IMasterTask.hpp"
#include "../../common/tasks/RtosTask.hpp"
#include "../../common/bridge/master_bridge/IMasterBridge.hpp"

/**
 * Concrete master task.
 *
 * Runs as a FreeRTOS task and coordinates all master-side application logic:
 * receiving status from slaves, applying presets, forwarding commands, etc.
 */
class MasterTask : public RtosTask, public IMasterTask
{
public:
    explicit MasterTask(IMasterBridge &masterBridge);
    ~MasterTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    IMasterBridge &masterBridge_;
};
