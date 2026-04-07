#pragma once

#include "esp_now_if.hpp"

#include <cstddef>
#include <functional>
#include <span>

/**
 * Interface for the ESP-NOW abstraction layer.
 *
 * Implementations wrap the ESP-IDF esp_now_* API (or a test double).
 * All data buffers are expressed as std::span<const uint8_t> so that callers
 * never need to pass a separate pointer + length pair.
 *
 * The callback signatures match the ESP-IDF 5.0+ / 6.0 forms:
 *   - Send callback:    (src_mac, status)
 *   - Receive callback: (recv_info, data, rssi)
 *
 * [[nodiscard]] is applied to every function that returns esp_err_t so that
 * callers cannot silently ignore errors.
 */
class IEspNow {
public:
    virtual ~IEspNow() = default;

    /** Initialise the ESP-NOW subsystem. */
    [[nodiscard]] virtual esp_err_t init() = 0;

    /** De-initialise the ESP-NOW subsystem. */
    [[nodiscard]] virtual esp_err_t deinit() = 0;

    /**
     * Register a peer device.
     * @param peer_info  Peer descriptor (address, channel, encryption config).
     */
    [[nodiscard]] virtual esp_err_t connectPeer(const esp_now_peer_info_t *peer_info) = 0;

    /**
     * Remove a previously registered peer device.
     * @param peer_addr  6-byte MAC address of the peer to remove.
     */
    [[nodiscard]] virtual esp_err_t disconnectPeer(const uint8_t *peer_addr) = 0;

    /**
     * Send a message via ESP-NOW.
     * @param data  Payload bytes expressed as a span (no separate length needed).
     */
    [[nodiscard]] virtual esp_err_t sendMessage(std::span<const uint8_t> data) = 0;

    /**
     * Register the send-complete callback (ESP-IDF 5.0+ / 6.0).
     * The callback is invoked after each transmission attempt with the
     * destination MAC address and the delivery status.
     * @param cb  Callback function; pass nullptr to unregister.
     */
    [[nodiscard]] virtual esp_err_t registerSendCallback(esp_now_send_cb_t cb) = 0;

    /**
     * Register the receive callback (ESP-IDF 5.0+ / 6.0).
     * The callback is invoked for every received ESP-NOW frame and receives the
     * full recv_info struct (source/destination address) plus RSSI.
     * @param cb  Callback function; pass nullptr to unregister.
     */
    [[nodiscard]] virtual esp_err_t registerReceiveCallback(esp_now_recv_cb_t cb) = 0;
};
