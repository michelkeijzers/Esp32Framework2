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
typedef struct {
    uint8_t peer_addr[6];
    uint8_t channel;
    wifi_second_chan_t second_channel;
} esp_now_peer_info_t;

#endif
