#pragma once

#include "../esp/esp_error/esp_error_if.hpp"

class IRtosTask
{
  public:
    virtual ~IRtosTask() = default;

    virtual esp_err_t init() = 0;
    virtual esp_err_t start() = 0;
};