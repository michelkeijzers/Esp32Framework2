#pragma once

#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"
#include "../../../common/esp/esp_error/esp_error_if.hpp"

/**
 * Interface for the /api/v1/configuration REST endpoints.
 *
 * GET  /api/v1/configuration                              — read current config
 * PUT  /api/v1/configuration                              — update config
 * PUT  /api/v1/configuration_presets_circular_navigation  — toggle circular navigation
 */
class IApiConfiguration
{
public:
    virtual ~IApiConfiguration() = default;

    virtual esp_err_t get_configuration_handler(httpd_req_t *req)       = 0;
    virtual esp_err_t put_configuration_handler(httpd_req_t *req)       = 0;
    virtual esp_err_t set_circular_navigation_handler(httpd_req_t *req) = 0;
};
