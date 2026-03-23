
#include "ApiSecurity.hpp"
#include "../../esp/esp_http_server/IEspHttpServer.hpp"

ApiSecurity::ApiSecurity(IEspHttpServer& espHttpServer) : espHttpServer_(espHttpServer) {}
ApiSecurity::~ApiSecurity() = default;

esp_err_t ApiSecurity::esp_now_key_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return 0;
}

esp_err_t ApiSecurity::wifi_password_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return 0;
}
