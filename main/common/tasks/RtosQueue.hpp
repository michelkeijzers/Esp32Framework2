#pragma once

#include <cstddef>
#include "IRtosQueue.hpp"

// FreeRTOS-like, generic, extensible queue
class RtosQueue : public IRtosQueue 
{
public:
    RtosQueue(size_t itemSize, size_t length);
    ~RtosQueue() override;

    bool Create();

    bool send(const void* item, uint32_t timeoutMs) override;
    bool receive(void* item, uint32_t timeoutMs) override;
    size_t itemSize() const override;
    size_t capacity() const override;
    size_t count() const override;

private:
    size_t m_itemSize;
    size_t m_length;
    void* m_queueHandle; // Opaque handle to underlying RTOS queue
};
