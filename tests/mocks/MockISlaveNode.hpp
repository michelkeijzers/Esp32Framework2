#pragma once
#include <gmock/gmock.h>

#include "ISlaveNode.hpp"

class MockISlaveNode : public ISlaveNode {
   public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
};
