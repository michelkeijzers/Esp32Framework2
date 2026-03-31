
#include "ApiConfig.hpp"
#include "IEspHttpServer.hpp"
#include "ApiResponse.hpp"
#include "ApiValidator.hpp"
#include <cstring>

ApiConfig::ApiConfig(IEspHttpServer& espHttpServer, IEspNvs& nvsManager) 
    : espHttpServer_(espHttpServer), nvsManager_(nvsManager) {}

ApiConfig::~ApiConfig() = default;

esp_err_t ApiConfig::get_config_handler(httpd_req_t *req) {
    char response[MAX_RESPONSE_SIZE];
    char ssid[API_MAX_SSID_LEN + 1] = {0};
    char device_name[MAX_DEVICE_NAME_LEN + 1] = {0};
    size_t len = 0;

    // Open NVS handle
    nvs_handle_t nvs_handle;
    esp_err_t err = nvsManager_.nvs_open(CONFIG_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        int written = ApiResponse::build_error_response(response, MAX_RESPONSE_SIZE, 
                                                       ApiResponse::StatusCode::INTERNAL_SERVER_ERROR, 
                                                       "Failed to open NVS");
        espHttpServer_.httpd_resp_send(req, response, written);
        return ESP_OK;
    }

    // Read configuration from NVS
    len = API_MAX_SSID_LEN;
    nvsManager_.nvs_get_str(nvs_handle, KEY_WIFI_SSID, ssid, &len);
    
    len = MAX_DEVICE_NAME_LEN;
    nvsManager_.nvs_get_str(nvs_handle, KEY_DEVICE_NAME, device_name, &len);
    
    nvsManager_.nvs_close(nvs_handle);

    // Build response
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    
    // Format: {"wifi_ssid":"MyNetwork","device_name":"master","espnow_configured":true}
    snprintf(response, MAX_RESPONSE_SIZE, 
             "{\"wifi_ssid\":\"%s\",\"device_name\":\"%s\",\"espnow_configured\":true}",
             ssid, device_name);
    
    int written = ApiResponse::build_success_response(response, MAX_RESPONSE_SIZE, response);
    if (written > 0) {
        espHttpServer_.httpd_resp_send(req, response, written);
    } else {
        ApiResponse::build_error_response(response, MAX_RESPONSE_SIZE,
                                         ApiResponse::StatusCode::INTERNAL_SERVER_ERROR,
                                         "Response too large");
        espHttpServer_.httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    }
    
    return ESP_OK;
}

esp_err_t ApiConfig::put_config_handler(httpd_req_t *req) {
    char buffer[512] = {0};
    int total_len = 0;

    int cur_len = 0;
    while (cur_len < sizeof(buffer) - 1) {
        cur_len = espHttpServer_.httpd_req_recv(req, buffer + total_len, sizeof(buffer) - 1 - total_len);
        if (cur_len <= 0) {
            if (cur_len == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            break;
        }
        total_len += cur_len;
    }
    buffer[total_len] = '\0';

    char response[MAX_RESPONSE_SIZE];
    char ssid[API_MAX_SSID_LEN + 1] = {0};
    char device_name[MAX_DEVICE_NAME_LEN + 1] = {0};
    bool has_ssid = false, has_device_name = false;
    
    // Extract wifi_ssid
    const char* ssid_start = strstr(buffer, "\"wifi_ssid\":\"");
    if (ssid_start) {
        ssid_start += 13; // Length of "\"wifi_ssid\":\""
        const char* ssid_end = strchr(ssid_start, '"');
        if (ssid_end && (ssid_end - ssid_start) <= API_MAX_SSID_LEN) {
            strncpy(ssid, ssid_start, ssid_end - ssid_start);
            ssid[ssid_end - ssid_start] = '\0';
            has_ssid = true;
        }
    }

    // Extract device_name
    const char* name_start = strstr(buffer, "\"device_name\":\"");
    if (name_start) {
        name_start += 15; // Length of "\"device_name\":\""
        const char* name_end = strchr(name_start, '"');
        if (name_end && (name_end - name_start) <= MAX_DEVICE_NAME_LEN) {
            strncpy(device_name, name_start, name_end - name_start);
            device_name[name_end - name_start] = '\0';
            has_device_name = true;
        }
    }

    // Validate inputs
    if (has_ssid && !ApiValidator::is_valid_ssid(ssid)) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        ApiResponse::build_error_response(response, MAX_RESPONSE_SIZE,
                                         ApiResponse::StatusCode::BAD_REQUEST,
                                         "Invalid SSID (1-32 characters)");
        espHttpServer_.httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    if (has_device_name && !ApiValidator::is_valid_device_name(device_name)) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        ApiResponse::build_error_response(response, MAX_RESPONSE_SIZE,
                                         ApiResponse::StatusCode::BAD_REQUEST,
                                         "Invalid device name (alphanumeric + underscore, 1-32 chars)");
        espHttpServer_.httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    // Open NVS handle for writing
    nvs_handle_t nvs_handle;
    esp_err_t err = nvsManager_.nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        ApiResponse::build_error_response(response, MAX_RESPONSE_SIZE,
                                         ApiResponse::StatusCode::INTERNAL_SERVER_ERROR,
                                         "Failed to open NVS");
        espHttpServer_.httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    // Write to NVS
    if (has_ssid) {
        nvsManager_.nvs_set_str(nvs_handle, KEY_WIFI_SSID, ssid);
    }
    if (has_device_name) {
        nvsManager_.nvs_set_str(nvs_handle, KEY_DEVICE_NAME, device_name);
    }
    
    nvsManager_.nvs_commit(nvs_handle);
    nvsManager_.nvs_close(nvs_handle);

    // Return success response
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    snprintf(response, MAX_RESPONSE_SIZE, 
             "{\"message\":\"Configuration updated\",\"wifi_ssid\":\"%s\",\"device_name\":\"%s\"}",
             ssid, device_name);
    
    int written = ApiResponse::build_success_response(response, MAX_RESPONSE_SIZE, response);
    if (written > 0) {
        espHttpServer_.httpd_resp_send(req, response, written);
    } else {
        ApiResponse::build_error_response(response, MAX_RESPONSE_SIZE,
                                         ApiResponse::StatusCode::INTERNAL_SERVER_ERROR,
                                         "Response too large");
        espHttpServer_.httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    }
    
    return ESP_OK;
}
