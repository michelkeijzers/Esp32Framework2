#pragma once

#include <cstddef>
#include <cstdint>

#include "../esp/free_rtos/free_rtos_if.hpp"

// Interface for RTOS-like queue (for DI)
class IRtosQueue {
public:
    virtual ~IRtosQueue() = default;
    virtual bool create() = 0;
    virtual QueueHandle_t getHandle() = 0;
    virtual bool send(const void* item, uint32_t timeoutMs) = 0;
    virtual bool receive(void* item, uint32_t timeoutMs) = 0;
    virtual size_t itemSize() const = 0;
    virtual size_t capacity() const = 0;
    virtual size_t count() const = 0;
};