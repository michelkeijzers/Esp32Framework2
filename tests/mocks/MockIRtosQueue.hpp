#pragma once

#include <gmock/gmock.h>
#include "tasks/IRtosQueue.hpp"

class MockIRtosQueue : public IRtosQueue {
public:
    ~MockIRtosQueue() override = default;
    MOCK_METHOD(bool, create, (), (override));
    MOCK_METHOD(QueueHandle_t, getHandle, (), (override));
    MOCK_METHOD(bool, send, (const void* item, uint32_t timeoutMs), (override));
    MOCK_METHOD(bool, receive, (void* item, uint32_t timeoutMs), (override));
    MOCK_METHOD(size_t, itemSize, (), (const, override));
    MOCK_METHOD(size_t, capacity, (), (const, override));
    MOCK_METHOD(size_t, count, (), (const, override));
};
