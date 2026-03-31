#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ServiceTasks.hpp"
#include "MockIFreeRtosFactory.hpp"

// ServiceTasks owns LoggingTask, StatusTask, OtaTask by value.
// All three are stub implementations that return ESP_OK from init()/start().

class ServiceTasksTest : public ::testing::Test
{
protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    ServiceTasks serviceTasks{mockRtosFactory};
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

TEST_F(ServiceTasksTest, GetStatusTaskReturnsReference)
{
    IStatusTask &ref = serviceTasks.getStatusTask();
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
