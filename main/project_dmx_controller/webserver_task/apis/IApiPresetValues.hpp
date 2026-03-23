#pragma once
#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"
#include "../../../common/esp/esp_error/esp_error_if.hpp"

class IApiPresetValues {
public:
    virtual ~IApiPresetValues() = default;

    virtual esp_err_t get_preset_values_handler(httpd_req_t *req) = 0;
    virtual esp_err_t set_preset_value_handler(httpd_req_t *req) = 0;
};
