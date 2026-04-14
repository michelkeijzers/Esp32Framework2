#pragma once

#include "../../common/esp/esp_error/esp_error_if.hpp"

/**
 * Interface for the master node.
 *
 * The master node is the top-level coordinator. It owns the bridge,
 * master task, web server task, service tasks, function tasks, and slave nodes.
 */
class IMasterNode {
   public:
    virtual ~IMasterNode() = default;

    /**
     * Initialise the master node and all owned components.
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the master node (starts all tasks).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;
};
