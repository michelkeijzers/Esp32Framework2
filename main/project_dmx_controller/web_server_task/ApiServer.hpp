#pragma once

#include "IApiServer.hpp"
#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"
#include "../../../common/web_server_task/apis/StaticFileHandler.hpp"
#include "../../../common/web_server_task/apis/ApiStatus.hpp"
#include "../../../common/web_server_task/apis/ApiNodes.hpp"
#include "../../../common/web_server_task/apis/ApiSystem.hpp"
#include "../../../common/web_server_task/apis/ApiFirmware.hpp"
#include "../../../common/web_server_task/apis/ApiSecurity.hpp"
#include "../../../common/web_server_task/apis/ApiLogging.hpp"
#include "../../../common/context/IEspFactory.hpp"
#include "../../../common/context/ICommonApiFactory.hpp"

class IEspLittleFs;
class IEspHttpServer;
class IEspLogger;
class NodesStaticInfo;

/**
 * Project API server implementation for DMX controller.
 */
class ApiServer : public IApiServer
{
public:
    ApiServer(IEspFactory &espFactory, ICommonApiFactory &commonApiFactory, const NodesStaticInfo &nodesStaticInfo);
    ~ApiServer() override;

    void start() override;
    void stop() override;

    static esp_err_t static_file_handler_thunk(httpd_req_t *req);

protected:
    IEspLittleFs &espLittleFs_;
    IEspHttpServer &espHttpServer_;
    IEspLogger &logger_;

    IApiStatus &apiStatus_;
    IApiNodes &apiNodes_;
    IApiSystem &apiSystem_;
    IApiFirmware &apiFirmware_;
    IApiSecurity &apiSecurity_;
    IApiLogging &apiLogging_;

    httpd_handle_t server;

    void mount_littlefs();
    void register_static_file_handler();
    void register_generic_endpoints();
    virtual void register_endpoints();

private:
    esp_err_t static_file_handler(httpd_req_t *req);
};
