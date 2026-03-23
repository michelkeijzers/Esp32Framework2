#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MasterTask.hpp"
#include "MockIMasterBridge.hpp"

using ::testing::Return;

// ─────────────────────────────────────────────────────────────
// init
// ─────────────────────────────────────────────────────────────

TEST(MasterTaskTest, InitDelegatesToMasterBridge)
{
    MockIMasterBridge mockBridge;
    MasterTask task(mockBridge, "master_task", 8192, 7);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, task.init());
}

TEST(MasterTaskTest, InitPropagatesBridgeFailure)
{
    MockIMasterBridge mockBridge;
    MasterTask task(mockBridge, "master_task", 8192, 7);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, task.init());
}

// ─────────────────────────────────────────────────────────────
// start
// ─────────────────────────────────────────────────────────────

TEST(MasterTaskTest, StartReturnsOk)
{
    MockIMasterBridge mockBridge;
    MasterTask task(mockBridge, "master_task", 8192, 7);

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    task.init();

    EXPECT_EQ(ESP_OK, task.start());
}
