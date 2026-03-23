#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SlaveTask.hpp"
#include "MockISlaveBridge.hpp"

using ::testing::Return;

TEST(SlaveTaskTest, InitDelegatesToSlaveBridge)
{
    MockISlaveBridge mockBridge;
    SlaveTask task(mockBridge);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, task.init());
}

TEST(SlaveTaskTest, InitPropagatesBridgeFailure)
{
    MockISlaveBridge mockBridge;
    SlaveTask task(mockBridge);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, task.init());
}

TEST(SlaveTaskTest, StartReturnsOk)
{
    MockISlaveBridge mockBridge;
    SlaveTask task(mockBridge);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    task.init();

    EXPECT_EQ(ESP_OK, task.start());
}
