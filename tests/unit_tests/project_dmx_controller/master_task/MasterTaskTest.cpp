#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MasterTask.hpp"
#include "MockIMasterBridge.hpp"
#include "MockIFreeRtosFactory.hpp"

using ::testing::Return;

// ─────────────────────────────────────────────────────────────
// init
// ─────────────────────────────────────────────────────────────

TEST(MasterTaskTest, InitDelegatesToMasterBridge)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIMasterBridge mockBridge;
    MasterTask task(mockRtosFactory, mockBridge, "master_task", 8192, 7);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, task.init());
}

TEST(MasterTaskTest, InitPropagatesBridgeFailure)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIMasterBridge mockBridge;
    MasterTask task(mockRtosFactory, mockBridge, "master_task", 8192, 7);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, task.init());
}

// ─────────────────────────────────────────────────────────────
// start
// ─────────────────────────────────────────────────────────────

TEST(MasterTaskTest, StartReturnsOk)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIMasterBridge mockBridge;
    MasterTask task(mockRtosFactory, mockBridge, "master_task", 8192, 7);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    task.init();

    EXPECT_EQ(ESP_OK, task.start());
}
