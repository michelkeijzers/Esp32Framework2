#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MasterNode.hpp"

#include "MockIFreeRtosFactory.hpp"
#include "MockIMasterBridge.hpp"
#include "MockIMasterTask.hpp"
#include "MockIWebserverTask.hpp"
#include "MockIServiceTasks.hpp"
#include "MockISubTask.hpp"
#include "MockISlaveNode.hpp"

using ::testing::InSequence;
using ::testing::Return;

TEST(MasterNodeTest, InitCallsAllComponentsInOrder)
{
    
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebserverTask webserverTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask1;
    MockISubTask subTask2;
    MockISlaveNode slaveNode1;
    MockISlaveNode slaveNode2;

    MasterNode node(rtosFactory, masterBridge, masterTask, webserverTask, serviceTasks,
                    {&subTask1, &subTask2}, {&slaveNode1, &slaveNode2});

    InSequence seq;
    EXPECT_CALL(masterTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webserverTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask1, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask2, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode1, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode2, init()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.init());
}

TEST(MasterNodeTest, InitStopsOnMasterTaskFailure)
{
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebserverTask webserverTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webserverTask, serviceTasks,
                    {&subTask}, {&slaveNode});

    EXPECT_CALL(masterTask, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(webserverTask, init()).Times(0);
    EXPECT_CALL(serviceTasks, init()).Times(0);
    EXPECT_CALL(subTask, init()).Times(0);
    EXPECT_CALL(slaveNode, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(MasterNodeTest, InitStopsOnSubTaskFailure)
{
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebserverTask webserverTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask1;
    MockISubTask subTask2;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webserverTask, serviceTasks,
                    {&subTask1, &subTask2}, {&slaveNode});

    InSequence seq;
    EXPECT_CALL(masterTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webserverTask, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask1, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(subTask2, init()).Times(0);
    EXPECT_CALL(slaveNode, init()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.init());
}

TEST(MasterNodeTest, StartCallsAllComponentsInOrder)
{
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebserverTask webserverTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask1;
    MockISubTask subTask2;
    MockISlaveNode slaveNode1;
    MockISlaveNode slaveNode2;

    MasterNode node(rtosFactory, masterBridge, masterTask, webserverTask, serviceTasks,
                    {&subTask1, &subTask2}, {&slaveNode1, &slaveNode2});

    InSequence seq;
    EXPECT_CALL(masterTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webserverTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask1, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask2, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode1, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode2, start()).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, node.start());
}

TEST(MasterNodeTest, StartStopsOnWebserverFailure)
{
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebserverTask webserverTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webserverTask, serviceTasks,
                    {&subTask}, {&slaveNode});

    EXPECT_CALL(masterTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webserverTask, start()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(serviceTasks, start()).Times(0);
    EXPECT_CALL(subTask, start()).Times(0);
    EXPECT_CALL(slaveNode, start()).Times(0);

    EXPECT_EQ(ESP_FAIL, node.start());
}

TEST(MasterNodeTest, StartStopsOnSlaveNodeFailure)
{
    ::testing::NiceMock<MockIFreeRtosFactory> rtosFactory;
    MockIMasterBridge masterBridge;
    MockIMasterTask masterTask;
    MockIWebserverTask webserverTask;
    MockIServiceTasks serviceTasks;
    MockISubTask subTask;
    MockISlaveNode slaveNode;

    MasterNode node(rtosFactory, masterBridge, masterTask, webserverTask, serviceTasks,
                    {&subTask}, {&slaveNode});

    InSequence seq;
    EXPECT_CALL(masterTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(webserverTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(serviceTasks, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(subTask, start()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(slaveNode, start()).WillOnce(Return(ESP_FAIL));

    EXPECT_EQ(ESP_FAIL, node.start());
}
