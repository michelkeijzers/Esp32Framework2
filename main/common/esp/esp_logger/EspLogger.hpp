#pragma once
#include "IEspLogger.hpp"

/**
 * Concrete implementation of Logger abstraction for ESP32
 * Wraps esp_log macros and provides a class-based interface
 */
class EspLogger : public IEspLogger {
public:
    EspLogger() = default;
    virtual ~EspLogger() = default;

    void log_error(const char* tag, const char* message) override;
    void log_info(const char* tag, const char* message) override;
    void log_warn(const char* tag, const char* message) override;
};
