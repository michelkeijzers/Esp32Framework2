// DEFINES FOR THE PROJECT AND NODE TO BUILD
#define BUILD_PROJECT_DMX_CONTROLLER

#include "common/esp/esp_now/EspNow.hpp"
#include "common/esp/esp_file_systems/EspLittleFs.hpp"
#include "common/esp/esp_http_server/EspHttpServer.hpp"
#include "common/esp/esp_nvs/EspNvs.hpp"
#include "common/esp/esp_logger/EspLogger.hpp"
#include "common/bridge/master_bridge/MasterBridge.hpp"
#include "common/context/Contexts.hpp"
#include "common/webserver_task/apis/ApiStatus.hpp"
#include "common/webserver_task/apis/ApiNodes.hpp"
#include "common/webserver_task/apis/ApiSystem.hpp"
#include "common/webserver_task/apis/ApiFirmware.hpp"
#include "common/webserver_task/apis/ApiSecurity.hpp"
#include "common/webserver_task/apis/ApiLogging.hpp"
#include "common/service_tasks/ServiceTasks.hpp"

#define BUILD_MASTER_NODE
//#define BUILD_SLAVE_NODE

#if defined(BUILD_PROJECT_DMX_CONTROLLER)
    #if defined(BUILD_MASTER_NODE)
        #include "project_dmx_controller/master_task/MasterNode.hpp"
        #include "project_dmx_controller/master_task/MasterTask.hpp"
        #include "project_dmx_controller/webserver_task/ApiServer.hpp"
        #include "project_dmx_controller/webserver_task/WebserverTask.hpp"
    #endif
#endif

extern "C" void app_main(void)
{
    #if defined(BUILD_MASTER_NODE)
        EspNow espNow;
        EspLittleFs espLittleFs;
        EspHttpServer espHttpServer;
        EspNvs espNvs;
        EspLogger espLogger;

        ApiStatus apiStatus(espHttpServer);
        ApiNodes apiNodes(espHttpServer);
        ApiSystem apiSystem(espHttpServer);
        ApiFirmware apiFirmware(espHttpServer);
        ApiSecurity apiSecurity(espHttpServer);
        ApiLogging apiLogging(espHttpServer);

        EspContexts espContexts{espLittleFs, espHttpServer, espNvs, espLogger};
        CommonApiContexts commonApiContexts{apiStatus, apiNodes, apiSystem, apiFirmware, apiSecurity, apiLogging};
        Contexts contexts{espContexts, commonApiContexts};

        MasterBridge masterBridge(espNow);

        MasterTask masterTask(masterBridge);
        ApiServer apiServer(contexts);
        WebserverTask webserverTask(apiServer);

        ServiceTasks serviceTasks;

        MasterNode masterNode(masterBridge, masterTask, webserverTask, serviceTasks, {}, {});
        masterNode.init();
        masterNode.start();
    #elif defined(BUILD_SLAVE_NODE)
    #endif
}
