#pragma once

#include "../../../common/esp/esp_error/esp_error_if.hpp"

/**
 * Interface for the project web server task.
 */
class IWebServerTask
{
public:
    virtual ~IWebServerTask() = default;

    virtual esp_err_t init() = 0;
    virtual esp_err_t start() = 0;
    virtual esp_err_t stop() = 0;
};
