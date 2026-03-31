#include "EspNvs.hpp"

#include <nvs.h>
#include <nvs_flash.h>

esp_err_t EspNvs::nvs_flash_init() {
    return ::nvs_flash_init();
}

esp_err_t EspNvs::nvs_open(const char *namespace_name, nvs_open_mode_t open_mode, nvs_handle_t *out_handle) {
    return ::nvs_open(namespace_name, open_mode, out_handle);
}

void EspNvs::nvs_close(nvs_handle_t handle) {
    ::nvs_close(handle);
}

esp_err_t EspNvs::nvs_get_i32(nvs_handle_t handle, const char *key, int32_t *out_value) {
    return ::nvs_get_i32(handle, key, out_value);
}

esp_err_t EspNvs::nvs_get_u32(nvs_handle_t handle, const char *key, uint32_t *out_value) {
    return ::nvs_get_u32(handle, key, out_value);
}

esp_err_t EspNvs::nvs_get_i64(nvs_handle_t handle, const char *key, int64_t *out_value) {
    return ::nvs_get_i64(handle, key, out_value);
}

esp_err_t EspNvs::nvs_get_u64(nvs_handle_t handle, const char *key, uint64_t *out_value) {
    return ::nvs_get_u64(handle, key, out_value);
}

esp_err_t EspNvs::nvs_get_str(nvs_handle_t handle, const char *key, char *out_value, size_t *length) {
    return ::nvs_get_str(handle, key, out_value, length);
}

esp_err_t EspNvs::nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length) {
    return ::nvs_get_blob(handle, key, out_value, length);
}

esp_err_t EspNvs::nvs_set_i32(nvs_handle_t handle, const char *key, int32_t value) {
    return ::nvs_set_i32(handle, key, value);
}

esp_err_t EspNvs::nvs_set_u32(nvs_handle_t handle, const char *key, uint32_t value) {
    return ::nvs_set_u32(handle, key, value);
}

esp_err_t EspNvs::nvs_set_i64(nvs_handle_t handle, const char *key, int64_t value) {
    return ::nvs_set_i64(handle, key, value);
}

esp_err_t EspNvs::nvs_set_u64(nvs_handle_t handle, const char *key, uint64_t value) {
    return ::nvs_set_u64(handle, key, value);
}

esp_err_t EspNvs::nvs_set_str(nvs_handle_t handle, const char *key, const char *value) {
    return ::nvs_set_str(handle, key, value);
}

esp_err_t EspNvs::nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length) {
    return ::nvs_set_blob(handle, key, value, length);
}

esp_err_t EspNvs::nvs_commit(nvs_handle_t handle) {
    return ::nvs_commit(handle);
}

esp_err_t EspNvs::nvs_erase_key(nvs_handle_t handle, const char *key) {
    return ::nvs_erase_key(handle, key);
}

esp_err_t EspNvs::nvs_erase_all(nvs_handle_t handle) {
    return ::nvs_erase_all(handle);
}
