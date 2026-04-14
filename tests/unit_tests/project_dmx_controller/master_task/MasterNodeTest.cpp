#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MasterNode.hpp"
#include "MockIFreeRtosFactory.hpp"
#include "MockIFunctionTask.hpp"
#include "MockIMasterBridge.hpp"
#include "MockIMasterServiceTasks.hpp"
#include "MockIMasterTask.hpp"
#include "MockISlaveNode.hpp"
#include "MockIWebServerTask.hpp"

using ::testing::InSequence;
using ::testing::Return;

TEST(MasterNodeTest, InitCallsAllComponentsInOrder) {
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebServerTask webServerTask;
    MockIMasterServiceTasks serviceTasks;
    MockIFunctionTask functionTask1;
    MockIFunctionTask functionTask2;
    MockISlaveNode slaveNode1;
    MockISlaveNode slaveNode2;

    MasterNode node(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks,
                    {&functionTask1, &functionTask2}, {&slaveNode1, &slaveNode2},
                    std::vector<TaskStatusInfo *>{});

    InSequence seq;
    EXPECT_CALL(masterTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webServerTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask1, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask2, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode1, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode2, init()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.init());
}

TEST(MasterNodeTest, InitStopsOnMasterTaskFailure) {
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebServerTask webServerTask;
    MockIMasterServiceTasks serviceTasks;
    MockIFunctionTask functionTask;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks,
                    {&functionTask}, {&slaveNode}, std::vector<TaskStatusInfo *>{});

    EXPECT_CALL(masterTask, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(webServerTask, init()).Times(0);
    EXPECT_CALL(serviceTasks, init()).Times(0);
    EXPECT_CALL(functionTask, init()).Times(0);
    EXPECT_CALL(slaveNode, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(MasterNodeTest, InitStopsOnFunctionTaskFailure) {
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebServerTask webServerTask;
    MockIMasterServiceTasks serviceTasks;
    MockIFunctionTask functionTask1;
    MockIFunctionTask functionTask2;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks,
                    {&functionTask1, &functionTask2}, {&slaveNode},
                    std::vector<TaskStatusInfo *>{});

    InSequence seq;
    EXPECT_CALL(masterTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webServerTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask1, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(functionTask2, init()).Times(0);
    EXPECT_CALL(slaveNode, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(MasterNodeTest, StartCallsAllComponentsInOrder) {
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebServerTask webServerTask;
    MockIMasterServiceTasks serviceTasks;
    MockIFunctionTask functionTask1;
    MockIFunctionTask functionTask2;
    MockISlaveNode slaveNode1;
    MockISlaveNode slaveNode2;

    MasterNode node(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks,
                    {&functionTask1, &functionTask2}, {&slaveNode1, &slaveNode2},
                    std::vector<TaskStatusInfo *>{});

    InSequence seq;
    EXPECT_CALL(masterTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webServerTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask1, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask2, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode1, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode2, start()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.start());
}

TEST(MasterNodeTest, StartStopsOnWebServerFailure) {
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebServerTask webServerTask;
    MockIMasterServiceTasks serviceTasks;
    MockIFunctionTask functionTask;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks,
                    {&functionTask}, {&slaveNode}, std::vector<TaskStatusInfo *>{});

    EXPECT_CALL(masterTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webServerTask, start()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(serviceTasks, start()).Times(0);
    EXPECT_CALL(functionTask, start()).Times(0);
    EXPECT_CALL(slaveNode, start()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.start());
}

TEST(MasterNodeTest, StartStopsOnSlaveNodeFailure) {
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebServerTask webServerTask;
    MockIMasterServiceTasks serviceTasks;
    MockIFunctionTask functionTask;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webServerTask, serviceTasks,
                    {&functionTask}, {&slaveNode}, std::vector<TaskStatusInfo *>{});

    InSequence seq;
    EXPECT_CALL(masterTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webServerTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode, start()).WillOnce(Return(ESP_FAIL));

    EXPECT_EQ(ESP_FAIL, node.start());
}
