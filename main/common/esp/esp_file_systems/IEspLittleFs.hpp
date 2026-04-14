#pragma once

#include "esp_littlefs_if.h"

class IEspLittleFs {
   public:
    IEspLittleFs() = default;
    virtual ~IEspLittleFs() = default;

    virtual esp_err_t esp_littlefs_mount(const esp_vfs_littlefs_conf_t* conf) = 0;
    virtual esp_err_t esp_littlefs_format(const char* partition_label) = 0;
};
