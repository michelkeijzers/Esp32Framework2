#pragma once

#include "../../common/esp/esp_error/esp_error_if.hpp"

/**
 * Interface for the master task.
 *
 * The master task runs the main application loop on the master node,
 * coordinating communication between slave nodes and service tasks.
 */
class IMasterTask {
   public:
    virtual ~IMasterTask() = default;

    /**
     * Initialise the master task.
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the master task (creates FreeRTOS task).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;
};
