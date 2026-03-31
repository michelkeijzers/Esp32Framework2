#pragma once
#include "IEspNow.hpp"

// Concrete implementation of ESP-NOW handler for ESP32
class EspNow : public IEspNow {
public:
    EspNow();
    ~EspNow();

    esp_err_t init() override;
    esp_err_t deinit() override;
    esp_err_t connectPeer(const esp_now_peer_info_t* peer_info) override;
    esp_err_t disconnectPeer(const uint8_t *peer_addr) override;
    esp_err_t sendMessage(const uint8_t *data, size_t len) override;

private:
    bool initialized_;
};
