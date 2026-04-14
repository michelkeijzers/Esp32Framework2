#pragma once
#include <string>

/**
 * Interface for ESP32 Logger abstraction
 * Provides simple logging interface with error, info, and warn levels
 * Safe to use in both ESP32 and unit test builds
 */
class IEspLogger {
   public:
    virtual ~IEspLogger() = default;

    virtual void log_error(const char* tag, const char* message) = 0;
    virtual void log_info(const char* tag, const char* message) = 0;
    virtual void log_warn(const char* tag, const char* message) = 0;
};
