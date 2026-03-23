
#include "ApiNodes.hpp"
#include "../../esp/esp_http_server/IEspHttpServer.hpp"

ApiNodes::ApiNodes(IEspHttpServer& espHttpServer) : espHttpServer_(espHttpServer) {}
ApiNodes::~ApiNodes() = default;

esp_err_t ApiNodes::get_nodes_info_handler(httpd_req_t *req) {
    const char *resp = "[{\"name\":\"Master\",\"mac_address\":\"24:6F:28:AA:BB:CC\"},{\"name\":\"Webserver\",\"mac_address\":\"24:6F:28:BB:CC:DD\"}]";
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return 0;
}

esp_err_t ApiNodes::save_nodes_info_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return 0;
}
