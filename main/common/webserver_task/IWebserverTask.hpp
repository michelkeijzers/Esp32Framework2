#pragma once

#include "../esp/esp_error/esp_error_if.hpp"

/**
 * Interface for the webserver task.
 *
 * The webserver task runs the HTTP server as a FreeRTOS task and owns the
 * API server instance.
 */
class IWebserverTask
{
public:
    virtual ~IWebserverTask() = default;

    /**
     * Initialise the webserver task.
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the webserver task (starts the HTTP server and FreeRTOS task).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;

    /**
     * Stop the webserver task.
     * @return ESP_OK on success.
     */
    virtual esp_err_t stop() = 0;
};
