#pragma once

#include "../esp/esp_error/esp_error_if.hpp"

/**
 * Interface for all function tasks.
 *
 * A function task is a self-contained FreeRTOS task that handles a specific
 * hardware peripheral or feature. Function tasks are owned by MasterNode or
 * SlaveNode via aggregation.
 *
 * Note: This interface intentionally does NOT inherit from IRtosTask.
 * The concrete FunctionTask class satisfies both by inheriting from RtosTask
 * and IFunctionTask independently, avoiding diamond inheritance.
 */
class IFunctionTask {
   public:
    virtual ~IFunctionTask() = default;

    /**
     * Initialise the function task (configure hardware, allocate buffers, etc).
     * @return ESP_OK on success.
     */
    virtual esp_err_t init() = 0;

    /**
     * Start the function task (create FreeRTOS task).
     * @return ESP_OK on success.
     */
    virtual esp_err_t start() = 0;
};