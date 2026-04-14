#pragma once

#include "../esp/esp_error/esp_error_if.hpp"

class ILoggingTask;
class IOtaTask;

/**
 * Container interface that groups all common service tasks.
 *
 * The service tasks run continuously in the background and provide
 * shared functionality (logging, OTA updates) to all
 * nodes in the system.
 */
class IServiceTasks {
   public:
    virtual ~IServiceTasks() = default;

    /**
     * Initialise all contained service tasks.
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start all contained service tasks (creates FreeRTOS tasks).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;

    virtual ILoggingTask &getLoggingTask() = 0;
    virtual IOtaTask &getOtaTask() = 0;
};
