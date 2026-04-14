#pragma once
#include "../../esp/esp_error/esp_error_if.hpp"
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "IApiNodes.hpp"

class IEspHttpServer;
class NodesStaticInfo;

class ApiNodes : public IApiNodes {
   public:
    ApiNodes(IEspHttpServer &espHttpServer);
    ~ApiNodes();

    void set_nodes_static_info(const NodesStaticInfo &nodesStaticInfo) override;
    esp_err_t get_nodes_info_handler(httpd_req_t *req) override;
    esp_err_t save_nodes_info_handler(httpd_req_t *req) override;

   private:
    IEspHttpServer &espHttpServer_;
    const NodesStaticInfo *nodesStaticInfo_;
};
