#pragma once

#ifdef ESP_PLATFORM
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_err.h"
#else
// Stand-in types for non-ESP32 platforms
#include <cstdint>
#include "../esp_error/esp_error_if.hpp"

#define ESP_NOW_MAX_TOTAL_PEER_NUM 20
#define ESP_NOW_MAX_DATA_LEN 250

typedef int wifi_second_chan_t;
typedef int esp_now_send_status_t;

typedef struct {
    uint8_t peer_addr[6];
    uint8_t channel;
    wifi_second_chan_t second_channel;
} esp_now_peer_info_t;

typedef void (*esp_now_send_cb_t)(const uint8_t *mac_addr, esp_now_send_status_t status);
typedef void (*esp_now_recv_cb_t)(const uint8_t *mac_addr, const uint8_t *data, int len);

inline esp_err_t esp_now_init(void) { return ESP_OK; }
inline esp_err_t esp_now_deinit(void) { return ESP_OK; }
inline esp_err_t esp_now_register_send_cb(esp_now_send_cb_t cb) { return ESP_OK; }
inline esp_err_t esp_now_register_recv_cb(esp_now_recv_cb_t cb) { return ESP_OK; }
inline esp_err_t esp_now_add_peer(const esp_now_peer_info_t *peer_info) { return ESP_OK; }
inline esp_err_t esp_now_send(const uint8_t *peer_addr, const uint8_t *data, size_t len) { return ESP_OK; }

#endif
