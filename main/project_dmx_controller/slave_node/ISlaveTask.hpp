#pragma once

#include "../../common/esp/esp_error/esp_error_if.hpp"

/**
 * Interface for the slave task.
 *
 * The slave task runs the main FreeRTOS loop on a slave node, processing
 * commands received from the master via the SlaveBridge.
 */
class ISlaveTask
{
public:
    virtual ~ISlaveTask() = default;

    /**
     * Initialise the slave task.
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the slave task (creates FreeRTOS task).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;
};
