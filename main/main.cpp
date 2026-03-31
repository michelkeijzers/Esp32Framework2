// DEFINES FOR THE PROJECT AND NODE TO BUILD
#define BUILD_PROJECT_DMX_CONTROLLER

#include "common/bridge/master_bridge/MasterBridge.hpp"
#include "common/context/EspNowFactory.hpp"
#include "common/service_tasks/ServiceTasks.hpp"

#define BUILD_MASTER_NODE
//#define BUILD_SLAVE_NODE

#if defined(BUILD_PROJECT_DMX_CONTROLLER)
    #if defined(BUILD_MASTER_NODE)
        #include "project_dmx_controller/master_task/MasterNode.hpp"
        #include "project_dmx_controller/master_task/MasterTask.hpp"
        #include "project_dmx_controller/master_task/DmxContextFactory.hpp"
#include "project_dmx_controller/web_server_task/ApiServer.hpp"
#include "project_dmx_controller/web_server_task/WebServerTask.hpp"
#include "common/tasks/RtosQueue.hpp"
#include "common/tasks/RtosQueueSet.hpp"
#endif
#endif

extern "C" void app_main(void)
{
    #if defined(BUILD_PROJECT_DMX_CONTROLLER)
        #if defined(BUILD_MASTER_NODE)
            DmxContextFactory dmxContextFactory;
            EspNowFactory espNowFactory;

            IContextFactory &contextFactory = dmxContextFactory.getContextFactory();
            IEspFactory &espFactory = contextFactory.getEspFactory();
            ICommonApiFactory &commonApiFactory = contextFactory.getCommonApiFactory();

            MasterBridge masterBridge = MasterBridge(espNowFactory.getEspNow());
            RtosQueue masterBridgeQueue = RtosQueue(64, 8); //TODO
            ApiServer apiServer = ApiServer(espFactory, commonApiFactory); //TODO
            WebServerTask webServerTask = WebServerTask(espFactory.getFreeRtosFactory(), apiServer, "web_server_task", 8192, 6);
            RtosQueue webServerQueue = RtosQueue(64, 8);   // TODO
            RtosQueueSet queueSet = RtosQueueSet(64 + 64); //TODO
            IFreeRtosFactory &rtosFactory = espFactory.getFreeRtosFactory();
            MasterTask masterTask(rtosFactory, masterBridge, masterBridgeQueue, webServerTask, webServerQueue, queueSet, "master_task", 8192, 5);
            ServiceTasks serviceTasks(rtosFactory);
            MasterNode masterNode(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks, {}, {});
            masterNode.init();
            masterNode.start();
        #elif defined(BUILD_SLAVE_NODE)
        #endif
    #endif // BUILD_PROJECT_DMX_CONTROLLER
}
