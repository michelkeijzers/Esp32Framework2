#pragma once
#include "IEspNvs.hpp"

// Concrete implementation of NVS abstraction for ESP32
class EspNvs : public IEspNvs {
public:
    EspNvs() = default;
    virtual ~EspNvs() = default;

    esp_err_t nvs_flash_init() override;

    esp_err_t nvs_open(const char *namespace_name, nvs_open_mode_t open_mode, nvs_handle_t *out_handle) override;
    void nvs_close(nvs_handle_t handle) override;

    esp_err_t nvs_get_i32(nvs_handle_t handle, const char *key, int32_t *out_value) override;
    esp_err_t nvs_get_u32(nvs_handle_t handle, const char *key, uint32_t *out_value) override;
    esp_err_t nvs_get_i64(nvs_handle_t handle, const char *key, int64_t *out_value) override;
    esp_err_t nvs_get_u64(nvs_handle_t handle, const char *key, uint64_t *out_value) override;
    esp_err_t nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length) override;
    esp_err_t nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length) override;

    esp_err_t nvs_set_i32(nvs_handle_t handle, const char *key, int32_t value) override;
    esp_err_t nvs_set_u32(nvs_handle_t handle, const char *key, uint32_t value) override;
    esp_err_t nvs_set_i64(nvs_handle_t handle, const char *key, int64_t value) override;
    esp_err_t nvs_set_u64(nvs_handle_t handle, const char *key, uint64_t value) override;
    esp_err_t nvs_set_str(nvs_handle_t handle, const char *key, const char *value) override;
    esp_err_t nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length) override;

    esp_err_t nvs_commit(nvs_handle_t handle) override;
    esp_err_t nvs_erase_key(nvs_handle_t handle, const char *key) override;
    esp_err_t nvs_erase_all(nvs_handle_t handle) override;
};
