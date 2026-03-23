#pragma once

#include "../../esp/esp_error/esp_error_if.hpp"
#include "../../esp/esp_now/esp_now_if.hpp"

#include <cstdint>
#include <vector>
#include <functional>

/**
 * Interface for the slave-side bridge in the Remote Proxy pattern.
 *
 * The SlaveBridge listens for incoming ESP-NOW messages from the master and
 * dispatches them to the appropriate handler on the slave node.
 */
class ISlaveBridge
{
public:
    virtual ~ISlaveBridge() = default;

    /**
     * Initialise the bridge (sets up ESP-NOW receive callback, etc).
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Register the master node's MAC address so that the slave can verify
     * the source of incoming messages.
     *
     * @param masterMac  6-byte MAC address of the master node.
     * @return ESP_OK on success.
     */
    virtual esp_err_t registerMaster(const esp_now_peer_info_t &masterPeerInfo) = 0;

    /**
     * Set the callback invoked when a message is received from the master.
     *
     * @param callback  Function called with the raw payload bytes.
     */
    virtual void setReceiveCallback(
        std::function<void(const std::vector<uint8_t> &)> callback) = 0;
};
