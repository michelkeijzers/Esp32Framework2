
#include "ApiLogging.hpp"

#include "../../esp/esp_http_server/IEspHttpServer.hpp"

ApiLogging::ApiLogging(IEspHttpServer& espHttpServer) : espHttpServer_(espHttpServer) {}
ApiLogging::~ApiLogging() = default;

esp_err_t ApiLogging::logging_handler(httpd_req_t* req) {
    espHttpServer_.httpd_resp_set_type(req, "text/event-stream");
    espHttpServer_.httpd_resp_send(req, "data: Log line 1\n\n", HTTPD_RESP_USE_STRLEN);
    return 0;
}
