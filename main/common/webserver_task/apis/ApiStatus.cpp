
#include "ApiStatus.hpp"
#include "../../esp/esp_http_server/IEspHttpServer.hpp"

ApiStatus::ApiStatus(IEspHttpServer& espHttpServer) : espHttpServer_(espHttpServer) {}
ApiStatus::~ApiStatus() = default;

esp_err_t ApiStatus::get_status_handler(httpd_req_t *req) {
    const char *resp = "[{\"name\":\"Master\",\"role\":\"Master\",\"node_type\":\"Webserver\",\"status_watchdog\":\"OK\"}]";
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return 0;
}

esp_err_t ApiStatus::get_status_stream_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "text/event-stream");
    const char *event = "data: [{\"name\":\"Master\",\"role\":\"Master\",\"node_type\":\"Webserver\",\"status_watchdog\":\"OK\"}]\n\n";
    espHttpServer_.httpd_resp_send(req, event, HTTPD_RESP_USE_STRLEN);
    return 0;
}
