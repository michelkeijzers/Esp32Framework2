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

            MasterBridge masterBridge(espNowFactory.getEspNow());

            IFreeRtosFactory &rtosFactory = espFactory.getFreeRtosFactory();

            MasterTask masterTask(rtosFactory, masterBridge, "master_task", 8192, 7);
            ApiServer apiServer(espFactory, commonApiFactory);
            WebserverTask webserverTask(rtosFactory, apiServer, "webserver_task", 8192, 6);

            ServiceTasks serviceTasks(rtosFactory);

            MasterNode masterNode(masterBridge, masterTask, webserverTask, serviceTasks, {}, {});
            masterNode.init();
            masterNode.start();
        #elif defined(BUILD_SLAVE_NODE)
        #endif
    #endif // BUILD_PROJECT_DMX_CONTROLLER
}
