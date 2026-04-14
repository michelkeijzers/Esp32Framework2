#include "EspNow.hpp"

#include "esp_now_if.hpp"

EspNow::EspNow() : initialized_(false) {}

EspNow::~EspNow() { (void)deinit(); }

esp_err_t EspNow::init() {
    esp_err_t ret = esp_now_init();
    if (ret == ESP_OK) {
        initialized_ = true;
    }
    return ret;
}

esp_err_t EspNow::deinit() {
    if (!initialized_) {
        return ESP_OK;
    }
    initialized_ = false;
    return esp_now_deinit();
}

esp_err_t EspNow::connectPeer(const esp_now_peer_info_t* peer_info) {
    return esp_now_add_peer(peer_info);
}

esp_err_t EspNow::disconnectPeer(const uint8_t* peer_addr) { return esp_now_del_peer(peer_addr); }

esp_err_t EspNow::sendMessage(std::span<const uint8_t> data) {
    // Broadcast to all registered peers (nullptr destination).
    return esp_now_send_payload(nullptr, data);
}

esp_err_t EspNow::registerSendCallback(esp_now_send_cb_t callback) {
    return esp_now_register_send_cb(callback);
}

esp_err_t EspNow::registerReceiveCallback(esp_now_recv_cb_t callback) {
    return esp_now_register_recv_cb(callback);
}
