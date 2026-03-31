#pragma once
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IApiSystem {
public:
    virtual ~IApiSystem() = default;

    virtual esp_err_t reboot_handler(httpd_req_t *req) = 0;
    virtual esp_err_t factory_reset_handler(httpd_req_t *req) = 0;
};
