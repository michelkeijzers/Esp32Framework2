#pragma once

#include "../esp/free_rtos/free_rtos_if.hpp"
#include "IRtosTask.hpp"
#include "../esp/esp_error/esp_error_if.hpp"

#include <cstdint>

class RtosTask : public IRtosTask
{
  public:
    RtosTask(const char *name, uint32_t stackSize, UBaseType_t priority);
    virtual ~RtosTask() = default;

    virtual esp_err_t init();
    virtual esp_err_t start();

  protected:
    const char *name_;
    uint32_t stackSize_;
    UBaseType_t priority_;
};