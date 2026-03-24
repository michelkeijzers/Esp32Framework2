#pragma once
#include <gmock/gmock.h>
#include "common/esp/esp_file_systems/IEspLittleFs.hpp"

class MockIEspLittleFs : public IEspLittleFs
{
public:
    MOCK_METHOD(esp_err_t, esp_littlefs_mount,  (const esp_vfs_littlefs_conf_t *), (override));
    MOCK_METHOD(esp_err_t, esp_littlefs_format, (const char *), (override));
};
