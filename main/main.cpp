// DEFINES FOR THE PROJECT AND NODE TO BUILD
#define BUILD_PROJECT_DMX_CONTROLLER

#include "common/bridge/master_bridge/MasterBridge.hpp"
#include "common/context/EspNowFactory.hpp"
#include "common/node/NodesStaticInfo.hpp"
#include "common/service_tasks/MasterServiceTasks.hpp"
#include "common/tasks/RtosQueue.hpp"
#include "common/tasks/RtosQueueSet.hpp"

#define BUILD_MASTER_NODE
// #define BUILD_SLAVE_NODE

#if defined(BUILD_PROJECT_DMX_CONTROLLER)
#if defined(BUILD_MASTER_NODE)
#include "project_dmx_controller/master_task/DmxContextFactory.hpp"
#include "project_dmx_controller/master_task/MasterNode.hpp"
#include "project_dmx_controller/master_task/MasterTask.hpp"
#include "project_dmx_controller/web_server_task/ApiServer.hpp"
#include "project_dmx_controller/web_server_task/WebServerTask.hpp"
#endif
#endif

extern "C" void app_main(void)
{
#if defined(BUILD_PROJECT_DMX_CONTROLLER)
#if defined(BUILD_MASTER_NODE)
    // Build static node information for the web server.
    NodeStaticInfo masterNodeStaticInfo(1, "Master Node", "1.0.0", "n.a.", "", "");
    masterNodeStaticInfo.addTaskStaticInfo(TaskStaticInfo(0, "Master Task"));
    masterNodeStaticInfo.addTaskStaticInfo(TaskStaticInfo(1, "Web Server Task"));

    NodesStaticInfo nodesStaticInfo;
    nodesStaticInfo.addNodeStaticInfo(masterNodeStaticInfo);

    DmxContextFactory dmxContextFactory;
    EspNowFactory espNowFactory;

    IContextFactory &contextFactory = dmxContextFactory.getContextFactory();
    IEspFactory &espFactory = contextFactory.getEspFactory();
    ICommonApiFactory &commonApiFactory = contextFactory.getCommonApiFactory();

    MasterBridge masterBridge = MasterBridge(espNowFactory.getEspNow());
    RtosQueue masterBridgeQueue = RtosQueue(64, 8);                                  // TODO
    ApiServer apiServer = ApiServer(espFactory, commonApiFactory, nodesStaticInfo);  // TODO
    WebServerTask webServerTask =
        WebServerTask(espFactory.getFreeRtosFactory(), apiServer, "web_server_task", 8192, 6);
    RtosQueue webServerQueue = RtosQueue(64, 8);   // TODO
    RtosQueueSet queueSet = RtosQueueSet(64 + 64); // TODO
    IFreeRtosFactory &rtosFactory = espFactory.getFreeRtosFactory();
    MasterTask masterTask(rtosFactory, masterBridge, masterBridgeQueue, webServerTask,
                          webServerQueue, queueSet, "master_task", 8192, 5);
    MasterServiceTasks serviceTasks(rtosFactory, masterNodeStaticInfo, 0);

    std::vector<TaskStatusInfo> statusTaskInfoStorage;
    statusTaskInfoStorage.reserve(masterNodeStaticInfo.tasksStaticInfo.size());
    for (const TaskStaticInfo &taskStaticInfo : masterNodeStaticInfo.tasksStaticInfo)
    {
        statusTaskInfoStorage.push_back(TaskStatusInfo{taskStaticInfo.taskId, taskStaticInfo.taskName});
    }

    std::vector<TaskStatusInfo *> statusTaskInfo;
    statusTaskInfo.reserve(statusTaskInfoStorage.size());
    for (TaskStatusInfo &taskStatusInfoEntry : statusTaskInfoStorage)
    {
        statusTaskInfo.push_back(&taskStatusInfoEntry);
    }
    MasterNode masterNode(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks, {}, {}, statusTaskInfo);
    masterNode.init();
    masterNode.start();
#elif defined(BUILD_SLAVE_NODE)
#endif
#endif // BUILD_PROJECT_DMX_CONTROLLER
}
