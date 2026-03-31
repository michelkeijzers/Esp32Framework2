#pragma once

#include "IApiConfiguration.hpp"
#include "../../../common/esp/esp_nvs/IEspNvs.hpp"

class IEspHttpServer;

/**
 * Handles the /api/v1/configuration REST endpoints.
 *
 * GET  /api/v1/configuration                              — read current config
 * PUT  /api/v1/configuration                              — update config
 * PUT  /api/v1/configuration_presets_circular_navigation  — toggle circular preset navigation
 */
class ApiConfiguration : public IApiConfiguration
{
public:
    ApiConfiguration(IEspHttpServer &espHttpServer, IEspNvs &nvsManager);
    ~ApiConfiguration() override;

    esp_err_t get_configuration_handler(httpd_req_t *req)       override;
    esp_err_t put_configuration_handler(httpd_req_t *req)       override;
    esp_err_t set_circular_navigation_handler(httpd_req_t *req) override;

private:
    IEspHttpServer &espHttpServer_;
    IEspNvs        &nvsManager_;

    static constexpr const char *CONFIG_NAMESPACE         = "config";
    static constexpr const char *KEY_WIFI_SSID            = "wifi_ssid";
    static constexpr const char *KEY_DEVICE_NAME          = "device_name";
    static constexpr const char *KEY_CIRCULAR_NAVIGATION  = "circ_nav";
    static constexpr size_t MAX_RESPONSE_SIZE  = 512;
    static constexpr size_t API_MAX_SSID_LEN       = 32;
    static constexpr size_t MAX_DEVICE_NAME_LEN = 32;
    static constexpr size_t REQUEST_BUF_SIZE   = 256;
};
