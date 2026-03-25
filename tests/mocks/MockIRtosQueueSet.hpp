#pragma once

#include <gmock/gmock.h>
#include "tasks/IRtosQueueSet.hpp"

class MockIRtosQueueSet : public IRtosQueueSet {
public:
    MOCK_METHOD(bool, xQueueCreateSet, (UBaseType_t eventQueueLength), (override));
    MOCK_METHOD(bool, xQueueAddToSet, (QueueSetMemberHandle_t queueOrSemaphore), (override));
    MOCK_METHOD(QueueSetMemberHandle_t, xQueueSelectFromSet, (TickType_t ticksToWait), (override));
    MOCK_METHOD(QueueSetHandle_t, getHandle, (), (const, override));
};
