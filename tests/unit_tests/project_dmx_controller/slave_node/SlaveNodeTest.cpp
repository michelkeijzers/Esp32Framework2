#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SlaveNode.hpp"
#include "MockISlaveBridge.hpp"
#include "MockISlaveTask.hpp"
#include "MockIServiceTasks.hpp"
#include "MockISubTask.hpp"

using ::testing::InSequence;
using ::testing::Return;

TEST(SlaveNodeTest, InitCallsAllComponentsInOrder)
{
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask1;
    MockISubTask subTask2;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&subTask1, &subTask2});

    InSequence seq;
    EXPECT_CALL(slaveTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask1, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask2, init()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.init());
}

TEST(SlaveNodeTest, InitStopsOnSlaveTaskFailure)
{
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&subTask});

    EXPECT_CALL(slaveTask, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(serviceTasks, init()).Times(0);
    EXPECT_CALL(subTask, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(SlaveNodeTest, InitStopsOnSubTaskFailure)
{
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask1;
    MockISubTask subTask2;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&subTask1, &subTask2});

    InSequence seq;
    EXPECT_CALL(slaveTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask1, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(subTask2, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(SlaveNodeTest, StartCallsAllComponentsInOrder)
{
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask1;
    MockISubTask subTask2;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&subTask1, &subTask2});

    InSequence seq;
    EXPECT_CALL(slaveTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask1, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask2, start()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.start());
}

TEST(SlaveNodeTest, StartStopsOnServiceFailure)
{
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&subTask});

    EXPECT_CALL(slaveTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(subTask, start()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.start());
}

TEST(SlaveNodeTest, StartStopsOnSubTaskFailure)
{
    MockISlaveBridge slaveBridge;
    MockISlaveTask slaveTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask;

    SlaveNode node(slaveBridge, slaveTask, serviceTasks, {&subTask});

    InSequence seq;
    EXPECT_CALL(slaveTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask, start()).WillOnce(Return(ESP_FAIL));

    EXPECT_EQ(ESP_FAIL, node.start());
}
