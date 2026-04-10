#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "common/node/NodeStaticInfo.hpp"
#include "ServiceTasks.hpp"
#include "MasterServiceTasks.hpp"
#include "MockIFreeRtosFactory.hpp"

// ServiceTasks owns LoggingTask and OtaTask by value.
// MasterServiceTasks extends it with StatusTask.

class ServiceTasksTest : public ::testing::Test
{
protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    NodeStaticInfo nodeStaticInfo{1, "Node", "1.0.0", "1.0.0", "", ""};
    ServiceTasks serviceTasks{mockRtosFactory, nodeStaticInfo, 0};
};

TEST_F(ServiceTasksTest, InitReturnsOk)
{
    EXPECT_EQ(ESP_OK, serviceTasks.init());
}

TEST_F(ServiceTasksTest, StartReturnsOk)
{
    EXPECT_EQ(ESP_OK, serviceTasks.start());
}

TEST_F(ServiceTasksTest, GetLoggingTaskReturnsReference)
{
    ILoggingTask &ref = serviceTasks.getLoggingTask();
    // Just verify the reference is valid (non-null)
    (void)ref;
    SUCCEED();
}

TEST_F(ServiceTasksTest, GetOtaTaskReturnsReference)
{
    IOtaTask &ref = serviceTasks.getOtaTask();
    (void)ref;
    SUCCEED();
}

TEST_F(ServiceTasksTest, InitThenStartBothSucceed)
{
    EXPECT_EQ(ESP_OK, serviceTasks.init());
    EXPECT_EQ(ESP_OK, serviceTasks.start());
}

class MasterServiceTasksTest : public ::testing::Test
{
protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    NodeStaticInfo nodeStaticInfo{1, "Node", "1.0.0", "1.0.0", "", ""};
    MasterServiceTasks serviceTasks{mockRtosFactory, nodeStaticInfo, 0};
};

TEST_F(MasterServiceTasksTest, InitReturnsOk)
{
    EXPECT_EQ(ESP_OK, serviceTasks.init());
}

TEST_F(MasterServiceTasksTest, StartReturnsOk)
{
    EXPECT_EQ(ESP_OK, serviceTasks.start());
}

TEST_F(MasterServiceTasksTest, GetStatusTaskReturnsReference)
{
    IStatusTask &ref = serviceTasks.getStatusTask();
    (void)ref;
    SUCCEED();
}
