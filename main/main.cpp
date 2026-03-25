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
        #include "project_dmx_controller/webserver_task/ApiServer.hpp"
        #include "project_dmx_controller/webserver_task/WebserverTask.hpp"
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
            WebserverTask webserverTask = WebserverTask(espFactory.getFreeRtosFactory(), apiServer, "webserver_task", 8192, 6);   
            RtosQueue webserverQueue = RtosQueue(64, 8); //TODO
            RtosQueueSet queueSet = RtosQueueSet(64 + 64); //TODO
            IFreeRtosFactory &rtosFactory = espFactory.getFreeRtosFactory();
            MasterTask masterTask(rtosFactory, masterBridge, masterBridgeQueue, webserverTask, webserverQueue, queueSet, "master_task", 8192, 5);
            ServiceTasks serviceTasks(rtosFactory);
            MasterNode masterNode(rtosFactory, masterBridge, masterTask, webserverTask, serviceTasks, {}, {});
            masterNode.init();
            masterNode.start();
        #elif defined(BUILD_SLAVE_NODE)
        #endif
    #endif // BUILD_PROJECT_DMX_CONTROLLER
}
