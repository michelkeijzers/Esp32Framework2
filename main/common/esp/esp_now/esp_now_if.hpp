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
    bool encrypt;   // ESP-NOW 2.0: enable per-peer encryption
} esp_now_peer_info_t;

// ESP-NOW 2.0 receive-info struct (ESP-IDF 5.0+ / 6.0).
// Replaces the bare mac_addr pointer in the old receive callback.
typedef struct {
    uint8_t *src_addr;  /*!< Source MAC address of the received packet. */
    uint8_t *des_addr;  /*!< Destination MAC address (unicast or broadcast). */
} esp_now_recv_info_t;

// Send callback: called after each ESP-NOW transmission.
typedef void (*esp_now_send_cb_t)(const uint8_t *mac_addr, esp_now_send_status_t status);

// Receive callback (ESP-IDF 5.0+ / 6.0 signature — the only form supported in ESP-IDF 6).
// NOTE: The old three-argument form (mac_addr, data, len) is removed in ESP-IDF 6.
//       Always use this four-argument form so that RSSI and address info are available.
typedef void (*esp_now_recv_cb_t)(const esp_now_recv_info_t *esp_now_info,
                                  const uint8_t             *data,
                                  int                        data_len,
                                  int                        rssi);

inline esp_err_t esp_now_init(void) { return ESP_OK; }
inline esp_err_t esp_now_deinit(void) { return ESP_OK; }
inline esp_err_t esp_now_register_send_cb(esp_now_send_cb_t cb) { (void)cb; return ESP_OK; }
inline esp_err_t esp_now_register_recv_cb(esp_now_recv_cb_t cb) { (void)cb; return ESP_OK; }
inline esp_err_t esp_now_add_peer(const esp_now_peer_info_t *peer_info) { (void)peer_info; return ESP_OK; }
inline esp_err_t esp_now_del_peer(const uint8_t *peer_addr) { (void)peer_addr; return ESP_OK; }
inline esp_err_t esp_now_send(const uint8_t *peer_addr, const uint8_t *data, size_t len)
{
    (void)peer_addr; (void)data; (void)len;
    return ESP_OK;
}

#endif
