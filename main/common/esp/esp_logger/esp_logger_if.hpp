#pragma once

#ifdef ESP_PLATFORM
// Real ESP-IDF logging macros
#include <esp_log.h>

#else
// Stub types and macros for non-ESP32 builds (PC, unit tests)
#define ESP_LOG_ERROR 0
#define ESP_LOG_WARN 1
#define ESP_LOG_INFO 2

#endif
