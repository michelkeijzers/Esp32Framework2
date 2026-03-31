#pragma once
#include <gmock/gmock.h>
#include "IEspNvs.hpp"

class MockIEspNvs : public IEspNvs
{
public:
    MOCK_METHOD(esp_err_t, nvs_flash_init, (), (override));
    MOCK_METHOD(esp_err_t, nvs_open, (const char *, nvs_open_mode_t, nvs_handle_t *), (override));
    MOCK_METHOD(void, nvs_close, (nvs_handle_t), (override));
    MOCK_METHOD(esp_err_t, nvs_get_i32, (nvs_handle_t, const char *, int32_t *), (override));
    MOCK_METHOD(esp_err_t, nvs_get_u32, (nvs_handle_t, const char *, uint32_t *), (override));
    MOCK_METHOD(esp_err_t, nvs_get_i64, (nvs_handle_t, const char *, int64_t *), (override));
    MOCK_METHOD(esp_err_t, nvs_get_u64, (nvs_handle_t, const char *, uint64_t *), (override));
    MOCK_METHOD(esp_err_t, nvs_get_str, (nvs_handle_t, const char *, char *, size_t *), (override));
    MOCK_METHOD(esp_err_t, nvs_get_blob, (nvs_handle_t, const char *, void *, size_t *), (override));
    MOCK_METHOD(esp_err_t, nvs_set_i32, (nvs_handle_t, const char *, int32_t), (override));
    MOCK_METHOD(esp_err_t, nvs_set_u32, (nvs_handle_t, const char *, uint32_t), (override));
    MOCK_METHOD(esp_err_t, nvs_set_i64, (nvs_handle_t, const char *, int64_t), (override));
    MOCK_METHOD(esp_err_t, nvs_set_u64, (nvs_handle_t, const char *, uint64_t), (override));
    MOCK_METHOD(esp_err_t, nvs_set_str, (nvs_handle_t, const char *, const char *), (override));
    MOCK_METHOD(esp_err_t, nvs_set_blob, (nvs_handle_t, const char *, const void *, size_t), (override));
    MOCK_METHOD(esp_err_t, nvs_commit, (nvs_handle_t), (override));
    MOCK_METHOD(esp_err_t, nvs_erase_key, (nvs_handle_t, const char *), (override));
    MOCK_METHOD(esp_err_t, nvs_erase_all, (nvs_handle_t), (override));
};
