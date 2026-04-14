#pragma once

#include "../esp/esp_error/esp_error_if.hpp"

/**
 * Interface for all nodes (MasterNode, SlaveNode).
 * Defines the lifecycle contract for nodes.
 */
class INode {
   public:
    virtual ~INode() = default;

    /**
     * Initialize the node and all its components.
     * @return ESP_OK on success, error code otherwise.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the node and all its active tasks.
     * @return ESP_OK on success, error code otherwise.
     */
    virtual esp_err_t start() = 0;
};
