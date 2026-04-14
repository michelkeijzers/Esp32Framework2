
#include "ApiFirmware.hpp"

#include "../../esp/esp_http_server/IEspHttpServer.hpp"

ApiFirmware::ApiFirmware(IEspHttpServer &espHttpServer) : espHttpServer_(espHttpServer) {}
ApiFirmware::~ApiFirmware() = default;

esp_err_t ApiFirmware::firmware_chunk_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return 0;
}

esp_err_t ApiFirmware::firmware_finish_handler(httpd_req_t *req) {
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return 0;
}
