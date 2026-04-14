#pragma once

#include <gmock/gmock.h>

#include "common/esp/free_rtos/IFreeRtosFactory.hpp"

class MockIFreeRtosFactory : public IFreeRtosFactory {
   public:
    MockIFreeRtosFactory() {
        ON_CALL(*this, createTask(::testing::_, ::testing::_, ::testing::_, ::testing::_,
                                  ::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(pdPASS));
    }

    MOCK_METHOD(BaseType_t, createTask,
                (TaskFunction_t, const char*, uint32_t, void*, UBaseType_t, TaskHandle_t*),
                (override));

    MOCK_METHOD(TickType_t, getTickCount, (), (override));
};
