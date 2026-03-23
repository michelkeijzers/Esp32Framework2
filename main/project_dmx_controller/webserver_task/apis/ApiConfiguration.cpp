#include "ApiConfiguration.hpp"
#include "../../../common/esp/esp_http_server/IEspHttpServer.hpp"
#include "../../../common/esp/esp_nvs/IEspNvs.hpp"

#include <cstring>
#include <cstdio>

ApiConfiguration::ApiConfiguration(IEspHttpServer &espHttpServer, IEspNvs &nvsManager)
    : espHttpServer_(espHttpServer)
    , nvsManager_(nvsManager)
{
}

ApiConfiguration::~ApiConfiguration() = default;

esp_err_t ApiConfiguration::get_configuration_handler(httpd_req_t *req)
{
    char ssid[MAX_SSID_LEN + 1]           = {0};
    char device_name[MAX_DEVICE_NAME_LEN + 1] = {0};
    uint32_t circular_nav                 = 0;
    size_t len                            = 0;

    nvs_handle_t nvs_handle;
    esp_err_t err = nvsManager_.nvs_open(CONFIG_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err == ESP_OK)
    {
        len = MAX_SSID_LEN;
        nvsManager_.nvs_get_str(nvs_handle, KEY_WIFI_SSID, ssid, &len);

        len = MAX_DEVICE_NAME_LEN;
        nvsManager_.nvs_get_str(nvs_handle, KEY_DEVICE_NAME, device_name, &len);

        nvsManager_.nvs_get_u32(nvs_handle, KEY_CIRCULAR_NAVIGATION, &circular_nav);

        nvsManager_.nvs_close(nvs_handle);
    }

    char response[MAX_RESPONSE_SIZE];
    snprintf(response, sizeof(response),
             "{\"wifi_ssid\":\"%s\",\"device_name\":\"%s\",\"circular navigation\":%s}",
             ssid, device_name, circular_nav ? "true" : "false");

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiConfiguration::put_configuration_handler(httpd_req_t *req)
{
    char buffer[REQUEST_BUF_SIZE] = {0};
    int  total_len = 0;
    int  cur_len   = 0;

    while (total_len < static_cast<int>(sizeof(buffer)) - 1)
    {
        cur_len = espHttpServer_.httpd_req_recv(req, buffer + total_len,
                                                sizeof(buffer) - 1 - total_len);
        if (cur_len <= 0)
        {
            if (cur_len == HTTPD_SOCK_ERR_TIMEOUT) continue;
            break;
        }
        total_len += cur_len;
    }
    buffer[total_len] = '\0';

    nvs_handle_t nvs_handle;
    esp_err_t err = nvsManager_.nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    // Parse and store wifi_ssid
    const char *ssid_start = strstr(buffer, "\"wifi_ssid\":\"");
    if (ssid_start)
    {
        ssid_start += 13;
        const char *ssid_end = strchr(ssid_start, '"');
        if (ssid_end && (ssid_end - ssid_start) <= static_cast<int>(MAX_SSID_LEN))
        {
            char ssid[MAX_SSID_LEN + 1] = {0};
            memcpy(ssid, ssid_start, ssid_end - ssid_start);
            nvsManager_.nvs_set_str(nvs_handle, KEY_WIFI_SSID, ssid);
        }
    }

    // Parse and store device_name
    const char *name_start = strstr(buffer, "\"device_name\":\"");
    if (name_start)
    {
        name_start += 15;
        const char *name_end = strchr(name_start, '"');
        if (name_end && (name_end - name_start) <= static_cast<int>(MAX_DEVICE_NAME_LEN))
        {
            char device_name[MAX_DEVICE_NAME_LEN + 1] = {0};
            memcpy(device_name, name_start, name_end - name_start);
            nvsManager_.nvs_set_str(nvs_handle, KEY_DEVICE_NAME, device_name);
        }
    }

    nvsManager_.nvs_commit(nvs_handle);
    nvsManager_.nvs_close(nvs_handle);

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiConfiguration::set_circular_navigation_handler(httpd_req_t *req)
{
    char buffer[REQUEST_BUF_SIZE] = {0};
    int  total_len = 0;
    int  cur_len   = 0;

    while (total_len < static_cast<int>(sizeof(buffer)) - 1)
    {
        cur_len = espHttpServer_.httpd_req_recv(req, buffer + total_len,
                                                sizeof(buffer) - 1 - total_len);
        if (cur_len <= 0)
        {
            if (cur_len == HTTPD_SOCK_ERR_TIMEOUT) continue;
            break;
        }
        total_len += cur_len;
    }
    buffer[total_len] = '\0';

    // Parse "circular navigation" boolean value
    bool value = false;
    if (strstr(buffer, "\"circular navigation\":true") ||
        strstr(buffer, "\"circular navigation\": true"))
    {
        value = true;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err = nvsManager_.nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    nvsManager_.nvs_set_u32(nvs_handle, KEY_CIRCULAR_NAVIGATION, value ? 1u : 0u);
    nvsManager_.nvs_commit(nvs_handle);
    nvsManager_.nvs_close(nvs_handle);

    char response[128];
    snprintf(response, sizeof(response),
             "{\"circular navigation\":%s}", value ? "true" : "false");

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
