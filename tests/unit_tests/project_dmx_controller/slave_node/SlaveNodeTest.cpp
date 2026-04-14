#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockIFunctionTask.hpp"
#include "MockISlaveBridge.hpp"
#include "MockISlaveServiceTasks.hpp"
#include "MockISlaveTask.hpp"
#include "SlaveNode.hpp"

using ::testing::InSequence;
using ::testing::Return;

TEST(SlaveNodeTest, InitCallsAllComponentsInOrder) {
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockISlaveServiceTasks serviceTasks;
    MockIFunctionTask functionTask1;
    MockIFunctionTask functionTask2;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&functionTask1, &functionTask2});

    InSequence seq;
    EXPECT_CALL(slaveTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask1, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask2, init()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.init());
}

TEST(SlaveNodeTest, InitStopsOnSlaveTaskFailure) {
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockISlaveServiceTasks serviceTasks;
    MockIFunctionTask functionTask;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&functionTask});

    EXPECT_CALL(slaveTask, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(serviceTasks, init()).Times(0);
    EXPECT_CALL(functionTask, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(SlaveNodeTest, InitStopsOnFunctionTaskFailure) {
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockISlaveServiceTasks serviceTasks;
    MockIFunctionTask functionTask1;
    MockIFunctionTask functionTask2;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&functionTask1, &functionTask2});

    InSequence seq;
    EXPECT_CALL(slaveTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask1, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(functionTask2, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(SlaveNodeTest, StartCallsAllComponentsInOrder) {
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockISlaveServiceTasks serviceTasks;
    MockIFunctionTask functionTask1;
    MockIFunctionTask functionTask2;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&functionTask1, &functionTask2});

    InSequence seq;
    EXPECT_CALL(slaveTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask1, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask2, start()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.start());
}

TEST(SlaveNodeTest, StartStopsOnServiceFailure) {
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockISlaveServiceTasks serviceTasks;
    MockIFunctionTask functionTask;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&functionTask});

    EXPECT_CALL(slaveTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(functionTask, start()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.start());
}

TEST(SlaveNodeTest, StartStopsOnFunctionTaskFailure) {
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockISlaveServiceTasks serviceTasks;
    MockIFunctionTask functionTask;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&functionTask});

    InSequence seq;
    EXPECT_CALL(slaveTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(functionTask, start()).WillOnce(Return(ESP_FAIL));

    EXPECT_EQ(ESP_FAIL, node.start());
}
