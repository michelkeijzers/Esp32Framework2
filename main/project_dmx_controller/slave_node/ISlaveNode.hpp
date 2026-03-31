#pragma once

#include "../../common/esp/esp_error/esp_error_if.hpp"

/**
 * Interface for a slave node.
 *
 * A slave node runs on a remote ESP32 unit and manages one or more
 * hardware sub tasks. It communicates with the master node over ESP-NOW
 * via a SlaveBridge.
 */
class ISlaveNode
{
public:
    virtual ~ISlaveNode() = default;

    /**
     * Initialise the slave node and all owned components.
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the slave node (starts all tasks).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;
};
