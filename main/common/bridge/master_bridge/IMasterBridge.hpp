#pragma once

#include <cstdint>
#include <span>

#include "../../esp/esp_error/esp_error_if.hpp"
#include "../../esp/esp_now/esp_now_if.hpp"

/**
 * Interface for the master-side bridge in the Remote Proxy pattern.
 *
 * The MasterBridge abstracts communication with slave nodes. Depending on the
 * implementation it either handles the request locally (for the master itself)
 * or forwards it over ESP-NOW to the corresponding SlaveBridge.
 *
 * [[nodiscard]] is applied to every function returning esp_err_t.
 * Data buffers use std::span<const uint8_t> instead of a raw pointer + length pair.
 */
class IMasterBridge {
   public:
    virtual ~IMasterBridge() = default;

    /**
     * Initialise the bridge (sets up ESP-NOW, registers receive callbacks, etc).
     * @return ESP_OK on success.
     */
    [[nodiscard]] virtual esp_err_t init() = 0;

    /**
     * Send a raw message payload to a target slave node.
     *
     * @param targetMac  6-byte MAC address of the destination slave.
     * @param data       Payload bytes.
     * @return ESP_OK on success.
     */
    [[nodiscard]] virtual esp_err_t sendMessage(
        std::span<const uint8_t, kEspNowMacAddressLength> targetMac,
        std::span<const uint8_t> data) = 0;

    /**
     * Broadcast a raw message payload to all registered slave nodes.
     *
     * @param data  Payload bytes.
     * @return ESP_OK on success.
     */
    [[nodiscard]] virtual esp_err_t broadcastMessage(std::span<const uint8_t> data) = 0;

    /**
     * Register a slave node's MAC address with this bridge.
     *
     * @param peerInfo  Peer descriptor (address, channel, encryption config).
     * @return ESP_OK on success.
     */
    [[nodiscard]] virtual esp_err_t addSlave(const esp_now_peer_info_t &peerInfo) = 0;
};
