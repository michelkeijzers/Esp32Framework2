#pragma once
#include <string>
#include <cstdint>
#include "../esp_error/esp_error_if.hpp"
#include "esp_nvs_if.hpp"

// Interface for ESP32 NVS (Non-Volatile Storage) abstraction
class IEspNvs {
public:
    virtual ~IEspNvs() = default;

    // Initialization and deinitialization
    virtual esp_err_t nvs_flash_init() = 0;

    // Handle operations
    virtual esp_err_t nvs_open(const char *namespace_name, nvs_open_mode_t open_mode, nvs_handle_t *out_handle) = 0;
    virtual void nvs_close(nvs_handle_t handle) = 0;

    // Get operations
    virtual esp_err_t nvs_get_i32(nvs_handle_t handle, const char *key, int32_t *out_value) = 0;
    virtual esp_err_t nvs_get_u32(nvs_handle_t handle, const char *key, uint32_t *out_value) = 0;
    virtual esp_err_t nvs_get_i64(nvs_handle_t handle, const char *key, int64_t *out_value) = 0;
    virtual esp_err_t nvs_get_u64(nvs_handle_t handle, const char *key, uint64_t *out_value) = 0;
    virtual esp_err_t nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length) = 0;
    virtual esp_err_t nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length) = 0;

    // Set operations
    virtual esp_err_t nvs_set_i32(nvs_handle_t handle, const char *key, int32_t value) = 0;
    virtual esp_err_t nvs_set_u32(nvs_handle_t handle, const char *key, uint32_t value) = 0;
    virtual esp_err_t nvs_set_i64(nvs_handle_t handle, const char *key, int64_t value) = 0;
    virtual esp_err_t nvs_set_u64(nvs_handle_t handle, const char *key, uint64_t value) = 0;
    virtual esp_err_t nvs_set_str(nvs_handle_t handle, const char *key, const char *value) = 0;
    virtual esp_err_t nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length) = 0;

    // Commit and erase operations
    virtual esp_err_t nvs_commit(nvs_handle_t handle) = 0;
    virtual esp_err_t nvs_erase_key(nvs_handle_t handle, const char *key) = 0;
    virtual esp_err_t nvs_erase_all(nvs_handle_t handle) = 0;
};
