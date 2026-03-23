#pragma once
#include "IApiLogging.hpp"
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IEspHttpServer;

class ApiLogging : public IApiLogging {
public:
    ApiLogging(IEspHttpServer& espHttpServer);
    ~ApiLogging();

    esp_err_t logging_handler(httpd_req_t *req) override;

private:
    IEspHttpServer& espHttpServer_;
};
