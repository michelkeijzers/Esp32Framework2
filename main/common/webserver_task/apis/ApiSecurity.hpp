#pragma once
#include "IApiSecurity.hpp"
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IEspHttpServer;

class ApiSecurity : public IApiSecurity {
public:
    ApiSecurity(IEspHttpServer& espHttpServer);
    ~ApiSecurity();

    esp_err_t esp_now_key_handler(httpd_req_t *req) override;
    esp_err_t wifi_password_handler(httpd_req_t *req) override;

private:
    IEspHttpServer& espHttpServer_;
};
