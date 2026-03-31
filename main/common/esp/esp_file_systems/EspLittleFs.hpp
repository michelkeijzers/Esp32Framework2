#pragma once
#include "IEspLittleFs.hpp"

class EspLittleFs : public IEspLittleFs {
public:
    EspLittleFs() = default;
    ~EspLittleFs() = default;

    esp_err_t esp_littlefs_mount(const esp_vfs_littlefs_conf_t *conf) override;
    esp_err_t esp_littlefs_format(const char* partition_label) override;
};
