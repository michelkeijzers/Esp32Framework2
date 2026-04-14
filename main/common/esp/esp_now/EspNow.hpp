#pragma once
#include "IEspNow.hpp"

/**
 * Concrete implementation of the ESP-NOW abstraction for the ESP32 target.
 *
 * Wraps the ESP-IDF esp_now_* C API and exposes it through the IEspNow
 * interface using modern C++ conventions (std::span, [[nodiscard]]).
 */
class EspNow : public IEspNow {
   public:
    EspNow();
    ~EspNow() override;

    [[nodiscard]] esp_err_t init() override;
    [[nodiscard]] esp_err_t deinit() override;
    [[nodiscard]] esp_err_t connectPeer(const esp_now_peer_info_t *peer_info) override;
    [[nodiscard]] esp_err_t disconnectPeer(const uint8_t *peer_addr) override;
    [[nodiscard]] esp_err_t sendMessage(std::span<const uint8_t> data) override;
    [[nodiscard]] esp_err_t registerSendCallback(esp_now_send_cb_t callback) override;
    [[nodiscard]] esp_err_t registerReceiveCallback(esp_now_recv_cb_t callback) override;

   private:
    bool initialized_;
};
