#pragma once

#include "IMasterTask.hpp"
#include "../../common/tasks/RtosTask.hpp"

class IWebserverTask; // Forward declaration
class IMasterBridge; // Forward declaration
class IRtosQueueSet; // Forward declaration
class IRtosQueue; // Forward declaration

/**
 * Concrete master task.
 *
 * Runs as a FreeRTOS task and coordinates all master-side application logic:
 * receiving status from slaves, applying presets, forwarding commands, etc.
 */
class MasterTask : public RtosTask, public IMasterTask
{
public:
    MasterTask(IFreeRtosFactory &freeRtosFactory, IMasterBridge &masterBridge, IRtosQueue& masterBridgeQueue, IWebserverTask& webServerTask, 
        IRtosQueue& webServerQueue, IRtosQueueSet& queueSet, const char *taskName, uint32_t stackSize, UBaseType_t taskPriority);
    ~MasterTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    IMasterBridge& masterBridge_;
    IRtosQueue& masterBridgeQueue_;
    IWebserverTask& webServerTask;
    IRtosQueue& webServerQueue_;
    IRtosQueueSet& queueSet_;
};
