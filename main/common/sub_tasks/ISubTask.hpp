#pragma once

#include "../esp/esp_error/esp_error_if.hpp"

/**
 * Interface for all sub tasks.
 *
 * A sub task is a self-contained FreeRTOS task that handles a specific
 * hardware peripheral or feature. Sub tasks are owned by MasterNode or
 * SlaveNode via aggregation.
 *
 * Note: This interface intentionally does NOT inherit from IRtosTask.
 * The concrete SubTask class satisfies both by inheriting from RtosTask
 * and ISubTask independently, avoiding diamond inheritance.
 */
class ISubTask
{
public:
    virtual ~ISubTask() = default;

    /**
     * Initialise the sub task (configure hardware, allocate buffers, etc).
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the sub task (create FreeRTOS task).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;
};
