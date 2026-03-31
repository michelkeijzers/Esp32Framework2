#include "common/esp/esp_nvs/EspNvs.hpp"

esp_err_t EspNvs::nvs_flash_init() { return ESP_OK; }

esp_err_t EspNvs::nvs_open(const char *, nvs_open_mode_t, nvs_handle_t *out_handle)
{
    if (out_handle != nullptr)
    {
        *out_handle = 1;
    }
    return ESP_OK;
}

void EspNvs::nvs_close(nvs_handle_t) {}

esp_err_t EspNvs::nvs_get_i32(nvs_handle_t, const char *, int32_t *) { return ESP_ERR_NOT_FOUND; }
esp_err_t EspNvs::nvs_get_u32(nvs_handle_t, const char *, uint32_t *) { return ESP_ERR_NOT_FOUND; }
esp_err_t EspNvs::nvs_get_i64(nvs_handle_t, const char *, int64_t *) { return ESP_ERR_NOT_FOUND; }
esp_err_t EspNvs::nvs_get_u64(nvs_handle_t, const char *, uint64_t *) { return ESP_ERR_NOT_FOUND; }
esp_err_t EspNvs::nvs_get_str(nvs_handle_t, const char *, char *, size_t *) { return ESP_ERR_NOT_FOUND; }
esp_err_t EspNvs::nvs_get_blob(nvs_handle_t, const char *, void *, size_t *) { return ESP_ERR_NOT_FOUND; }

esp_err_t EspNvs::nvs_set_i32(nvs_handle_t, const char *, int32_t) { return ESP_OK; }
esp_err_t EspNvs::nvs_set_u32(nvs_handle_t, const char *, uint32_t) { return ESP_OK; }
esp_err_t EspNvs::nvs_set_i64(nvs_handle_t, const char *, int64_t) { return ESP_OK; }
esp_err_t EspNvs::nvs_set_u64(nvs_handle_t, const char *, uint64_t) { return ESP_OK; }
esp_err_t EspNvs::nvs_set_str(nvs_handle_t, const char *, const char *) { return ESP_OK; }
esp_err_t EspNvs::nvs_set_blob(nvs_handle_t, const char *, const void *, size_t) { return ESP_OK; }

esp_err_t EspNvs::nvs_commit(nvs_handle_t) { return ESP_OK; }
esp_err_t EspNvs::nvs_erase_key(nvs_handle_t, const char *) { return ESP_OK; }
esp_err_t EspNvs::nvs_erase_all(nvs_handle_t) { return ESP_OK; }
