#pragma once
#include "../../../common/esp/esp_error/esp_error_if.hpp"
#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"

class IApiConfig {
   public:
    virtual ~IApiConfig() = default;

    virtual esp_err_t get_config_handler(httpd_req_t *req) = 0;
    virtual esp_err_t put_config_handler(httpd_req_t *req) = 0;
    virtual esp_err_t set_circular_navigation_handler(httpd_req_t *req) = 0;
};
