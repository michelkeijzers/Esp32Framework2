#pragma once
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IApiSecurity {
public:
    virtual ~IApiSecurity() = default;

    virtual esp_err_t esp_now_key_handler(httpd_req_t *req) = 0;
    virtual esp_err_t wifi_password_handler(httpd_req_t *req) = 0;
};
