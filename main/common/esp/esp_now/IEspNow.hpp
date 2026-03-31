#pragma once
#include "esp_now_if.hpp"

// Interface for ESP-NOW abstraction
class IEspNow {
public:
    virtual ~IEspNow() = default;

    virtual esp_err_t init() = 0;
    virtual esp_err_t deinit() = 0;
    virtual esp_err_t connectPeer(const esp_now_peer_info_t* peer_info) = 0;
    virtual esp_err_t disconnectPeer(const uint8_t *peer_addr) = 0;
    virtual esp_err_t sendMessage(const uint8_t *data, size_t len) = 0;
};
