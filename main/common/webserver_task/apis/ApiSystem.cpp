
#include "ApiSystem.hpp"
#include "../../esp/esp_http_server/IEspHttpServer.hpp"

ApiSystem::ApiSystem(IEspHttpServer& espHttpServer) : espHttpServer_(espHttpServer) {}
ApiSystem::~ApiSystem() = default;

esp_err_t ApiSystem::reboot_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "text/plain");
    espHttpServer_.httpd_resp_send(req, "", 0);
    return 0;
}

esp_err_t ApiSystem::factory_reset_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return 0;
}
