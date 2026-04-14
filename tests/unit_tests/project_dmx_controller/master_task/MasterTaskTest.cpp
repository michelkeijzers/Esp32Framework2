#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MasterTask.hpp"
#include "MockIFreeRtosFactory.hpp"
#include "MockIMasterBridge.hpp"
#include "MockIRtosQueue.hpp"
#include "MockIRtosQueueSet.hpp"
#include "MockIWebServerTask.hpp"

using ::testing::Return;

// ─────────────────────────────────────────────────────────────
// init
// ─────────────────────────────────────────────────────────────

TEST(MasterTaskTest, InitDelegatesToMasterBridge)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIMasterBridge mockBridge;
    MockIRtosQueue mockMasterBridgeQueue;
    MockIWebServerTask mockWebServerTask;
    MockIRtosQueue mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    ON_CALL(mockMasterBridgeQueue, create()).WillByDefault(Return(true));
    ON_CALL(mockWebServerQueue, create()).WillByDefault(Return(true));
    ON_CALL(mockQueueSet, createSet()).WillByDefault(Return(true));
    MasterTask task(
        mockRtosFactory,
        mockBridge,
        mockMasterBridgeQueue,
        mockWebServerTask,
        mockWebServerQueue,
        mockQueueSet,
        "master_task",
        8192,
        7
    );

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, task.init());
}

TEST(MasterTaskTest, InitPropagatesBridgeFailure)
{
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIMasterBridge mockBridge;
    MockIRtosQueue mockMasterBridgeQueue;
    MockIWebServerTask mockWebServerTask;
    MockIRtosQueue mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    ON_CALL(mockMasterBridgeQueue, create()).WillByDefault(Return(true));
    ON_CALL(mockWebServerQueue, create()).WillByDefault(Return(true));
    ON_CALL(mockQueueSet, createSet()).WillByDefault(Return(true));
    MasterTask task(
        mockRtosFactory,
        mockBridge,
        mockMasterBridgeQueue,
        mockWebServerTask,
        mockWebServerQueue,
        mockQueueSet,
        "master_task",
        8192,
        7
    );

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
    MockIRtosQueue mockMasterBridgeQueue;
    MockIWebServerTask mockWebServerTask;
    MockIRtosQueue mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    ON_CALL(mockMasterBridgeQueue, create()).WillByDefault(Return(true));
    ON_CALL(mockWebServerQueue, create()).WillByDefault(Return(true));
    ON_CALL(mockQueueSet, createSet()).WillByDefault(Return(true));
    // For start():
    ON_CALL(mockWebServerQueue, getHandle()).WillByDefault(Return(reinterpret_cast<QueueHandle_t>(0x1)));
    ON_CALL(mockQueueSet, addToSet(::testing::_)).WillByDefault(Return(true));
    MasterTask task(
        mockRtosFactory,
        mockBridge,
        mockMasterBridgeQueue,
        mockWebServerTask,
        mockWebServerQueue,
        mockQueueSet,
        "master_task",
        8192,
        7
    );

    EXPECT_CALL(mockBridge, init()).WillOnce(Return(ESP_OK));
    task.init();

    EXPECT_EQ(ESP_OK, task.start());
}
