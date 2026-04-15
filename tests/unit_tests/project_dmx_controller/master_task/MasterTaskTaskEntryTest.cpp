#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockIFreeRtosFactory.hpp"
#include "MockIMasterBridge.hpp"
#include "MockIRtosQueue.hpp"
#include "MockIRtosQueueSet.hpp"
#include "MockIWebServerTask.hpp"

#define private public
#include "MasterTask.hpp"
#undef private

using ::testing::Return;

TEST(MasterTaskTaskEntryTest, InitFailsWhenWebServerQueueCreateFails) {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIMasterBridge mockBridge;
    MockIRtosQueue mockMasterBridgeQueue;
    MockIWebServerTask mockWebServerTask;
    MockIRtosQueue mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    MasterTask task(mockRtosFactory, mockBridge, mockMasterBridgeQueue, mockWebServerTask,
                    mockWebServerQueue, mockQueueSet, "master_task", 8192, 7);

    EXPECT_CALL(mockWebServerQueue, create()).WillOnce(Return(false));
    EXPECT_CALL(mockBridge, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, task.init());
}

TEST(MasterTaskTaskEntryTest, StartFailsWhenAddingWebServerQueueToSetFails) {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    ::testing::NiceMock<MockIMasterBridge> mockBridge;
    MockIRtosQueue mockMasterBridgeQueue;
    MockIWebServerTask mockWebServerTask;
    MockIRtosQueue mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    MasterTask task(mockRtosFactory, mockBridge, mockMasterBridgeQueue, mockWebServerTask,
                    mockWebServerQueue, mockQueueSet, "master_task", 8192, 7);

    EXPECT_CALL(mockWebServerQueue, getHandle())
        .WillOnce(Return(reinterpret_cast<QueueHandle_t>(0x10)));
    EXPECT_CALL(mockQueueSet, addToSet(reinterpret_cast<QueueHandle_t>(0x10)))
        .WillOnce(Return(false));

    EXPECT_EQ(ESP_FAIL, task.start());
}

TEST(MasterTaskTaskEntryTest, StartFailsWhenAddingMasterBridgeQueueToSetFails) {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    ::testing::NiceMock<MockIMasterBridge> mockBridge;
    MockIRtosQueue mockMasterBridgeQueue;
    MockIWebServerTask mockWebServerTask;
    MockIRtosQueue mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    MasterTask task(mockRtosFactory, mockBridge, mockMasterBridgeQueue, mockWebServerTask,
                    mockWebServerQueue, mockQueueSet, "master_task", 8192, 7);

    EXPECT_CALL(mockWebServerQueue, getHandle())
        .WillOnce(Return(reinterpret_cast<QueueHandle_t>(0x10)));
    EXPECT_CALL(mockQueueSet, addToSet(reinterpret_cast<QueueHandle_t>(0x10)))
        .WillOnce(Return(true));
    EXPECT_CALL(mockMasterBridgeQueue, getHandle())
        .WillOnce(Return(reinterpret_cast<QueueHandle_t>(0x20)));
    EXPECT_CALL(mockQueueSet, addToSet(reinterpret_cast<QueueHandle_t>(0x20)))
        .WillOnce(Return(false));

    EXPECT_EQ(ESP_FAIL, task.start());
}

TEST(MasterTaskTaskEntryTest, TaskEntryReturnsWhenNoQueueWasActivated) {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    ::testing::NiceMock<MockIMasterBridge> mockBridge;
    ::testing::NiceMock<MockIRtosQueue> mockMasterBridgeQueue;
    ::testing::NiceMock<MockIWebServerTask> mockWebServerTask;
    ::testing::NiceMock<MockIRtosQueue> mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    MasterTask task(mockRtosFactory, mockBridge, mockMasterBridgeQueue, mockWebServerTask,
                    mockWebServerQueue, mockQueueSet, "master_task", 8192, 7);

    EXPECT_CALL(mockQueueSet, selectFromSet(portMAX_DELAY)).WillOnce(Return(nullptr));

    EXPECT_NO_THROW(task.taskEntry());
}

TEST(MasterTaskTaskEntryTest, TaskEntryConsumesWebServerQueueMessageWhenSelected) {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    ::testing::NiceMock<MockIMasterBridge> mockBridge;
    ::testing::NiceMock<MockIRtosQueue> mockMasterBridgeQueue;
    ::testing::NiceMock<MockIWebServerTask> mockWebServerTask;
    MockIRtosQueue mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    MasterTask task(mockRtosFactory, mockBridge, mockMasterBridgeQueue, mockWebServerTask,
                    mockWebServerQueue, mockQueueSet, "master_task", 8192, 7);
    auto webQueueHandle = reinterpret_cast<QueueHandle_t>(0x10);
    auto bridgeQueueHandle = reinterpret_cast<QueueHandle_t>(0x20);

    EXPECT_CALL(mockQueueSet, selectFromSet(portMAX_DELAY))
        .WillOnce(Return(webQueueHandle))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(mockWebServerQueue, getHandle()).WillRepeatedly(Return(webQueueHandle));
    EXPECT_CALL(mockMasterBridgeQueue, getHandle()).WillRepeatedly(Return(bridgeQueueHandle));
    EXPECT_CALL(mockWebServerQueue, receive(::testing::_, 0)).WillOnce(Return(true));

    EXPECT_NO_THROW(task.taskEntry());
}

TEST(MasterTaskTaskEntryTest, TaskEntryConsumesMasterBridgeQueueMessageWhenSelected) {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    ::testing::NiceMock<MockIMasterBridge> mockBridge;
    MockIRtosQueue mockMasterBridgeQueue;
    ::testing::NiceMock<MockIWebServerTask> mockWebServerTask;
    ::testing::NiceMock<MockIRtosQueue> mockWebServerQueue;
    MockIRtosQueueSet mockQueueSet;
    MasterTask task(mockRtosFactory, mockBridge, mockMasterBridgeQueue, mockWebServerTask,
                    mockWebServerQueue, mockQueueSet, "master_task", 8192, 7);
    auto webQueueHandle = reinterpret_cast<QueueHandle_t>(0x10);
    auto bridgeQueueHandle = reinterpret_cast<QueueHandle_t>(0x20);

    EXPECT_CALL(mockQueueSet, selectFromSet(portMAX_DELAY))
        .WillOnce(Return(bridgeQueueHandle))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(mockWebServerQueue, getHandle()).WillRepeatedly(Return(webQueueHandle));
    EXPECT_CALL(mockMasterBridgeQueue, getHandle()).WillRepeatedly(Return(bridgeQueueHandle));
    EXPECT_CALL(mockMasterBridgeQueue, receive(::testing::_, 0)).WillOnce(Return(true));

    EXPECT_NO_THROW(task.taskEntry());
}
