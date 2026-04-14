#pragma once

#include "../esp/esp_error/esp_error_if.hpp"

// Forward declaration
class IRtosQueue;

class IRtosTask {
   public:
    virtual ~IRtosTask() = default;

    virtual esp_err_t init() = 0;
    virtual esp_err_t start() = 0;

    // Create and add a queue to the task
    virtual IRtosQueue* createQueue(size_t itemSize, size_t length) = 0;

    virtual TickType_t getTickCount() = 0;
};