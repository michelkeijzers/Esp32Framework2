
#pragma once
#include "../../../common/esp/esp_error/esp_error_if.hpp"
#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"
#include "../../../common/esp/esp_nvs/IEspNvs.hpp"
#include "IApiConfig.hpp"

class IEspHttpServer;
class IEspNvs;

/**
 * Configuration API Endpoints
 *
 * GET /api/v1/config - Returns current configuration (Wi-Fi SSID, device name, etc.)
 * POST /api/v1/config - Updates configuration (Wi-Fi SSID, password, ESP-NOW key, device name)
 */
class ApiConfig : public IApiConfig {
   public:
    ApiConfig(IEspHttpServer& espHttpServer, IEspNvs& nvsManager);
    ~ApiConfig();

    esp_err_t get_config_handler(httpd_req_t* req) override;
    esp_err_t put_config_handler(httpd_req_t* req) override;
    esp_err_t set_circular_navigation_handler(httpd_req_t* req) override { return ESP_OK; }

   private:
    IEspHttpServer& espHttpServer_;
    IEspNvs& nvsManager_;

    // NVS namespace for configuration
    static constexpr const char* CONFIG_NAMESPACE = "config";

    // NVS keys
    static constexpr const char* KEY_WIFI_SSID = "wifi_ssid";
    static constexpr const char* KEY_WIFI_PASSWORD = "wifi_password";
    static constexpr const char* KEY_ESPNOW_KEY = "espnow_key";
    static constexpr const char* KEY_DEVICE_NAME = "device_name";

    // Buffer sizes
    static constexpr size_t MAX_RESPONSE_SIZE = 1024;
    static constexpr size_t API_MAX_SSID_LEN = 32;
    static constexpr size_t MAX_PASSWORD_LEN = 63;
    static constexpr size_t MAX_ESPNOW_KEY_LEN = 32;  // 16 bytes hex-encoded
    static constexpr size_t MAX_DEVICE_NAME_LEN = 32;
};
