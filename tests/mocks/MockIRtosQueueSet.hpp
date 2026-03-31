#pragma once

#include <gmock/gmock.h>
#include "tasks/IRtosQueueSet.hpp"

class MockIRtosQueueSet : public IRtosQueueSet {
public:
    MockIRtosQueueSet(uint32_t setLength = 1) : IRtosQueueSet(setLength) {}
    ~MockIRtosQueueSet() override = default;
    MOCK_METHOD(bool, createSet, (), (override));
    MOCK_METHOD(bool, addToSet, (QueueSetMemberHandle_t queueOrSemaphore), (override));
    MOCK_METHOD(QueueSetMemberHandle_t, selectFromSet, (TickType_t ticksToWait), (override));
    MOCK_METHOD(QueueSetHandle_t, getHandle, (), (const, override));
};
