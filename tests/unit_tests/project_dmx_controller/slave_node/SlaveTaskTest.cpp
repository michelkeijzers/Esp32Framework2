#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SlaveTask.hpp"
#include "MockISlaveBridge.hpp"
#include "MockIFreeRtosFactory.hpp"

using ::testing::Return;

TEST(SlaveTaskTest, InitDelegatesToSlaveBridge)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockISlaveBridge mockBridge;
    SlaveTask task(mockRtosFactory, mockBridge);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, task.init());
}

TEST(SlaveTaskTest, InitPropagatesBridgeFailure)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockISlaveBridge mockBridge;
    SlaveTask task(mockRtosFactory, mockBridge);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, task.init());
}

TEST(SlaveTaskTest, StartReturnsOk)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockISlaveBridge mockBridge;
    SlaveTask task(mockRtosFactory, mockBridge);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    task.init();

    EXPECT_EQ(ESP_OK, task.start());
}
