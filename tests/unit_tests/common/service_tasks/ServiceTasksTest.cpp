#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MasterServiceTasks.hpp"
#include "MockIFreeRtosFactory.hpp"
#include "ServiceTasks.hpp"
#include "common/node/NodeStaticInfo.hpp"
#include "logging_task/LoggingTask.hpp"
#include "ota_task/OtaTask.hpp"

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

class ExposedLoggingTask : public LoggingTask {
   public:
    explicit ExposedLoggingTask(IFreeRtosFactory &freeRtosFactory) : LoggingTask(freeRtosFactory) {}
    void callTaskEntry() { taskEntry(); }
};

class ExposedOtaTask : public OtaTask {
   public:
    explicit ExposedOtaTask(IFreeRtosFactory &freeRtosFactory) : OtaTask(freeRtosFactory) {}
    void callTaskEntry() { taskEntry(); }
};

// ServiceTasks owns LoggingTask and OtaTask by value.
// MasterServiceTasks extends it with StatusTask.

class ServiceTasksTest : public ::testing::Test {
   protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    NodeStaticInfo nodeStaticInfo{1, "Node", "1.0.0", "1.0.0", "", ""};
    ServiceTasks serviceTasks{mockRtosFactory, nodeStaticInfo, 0};
};

TEST_F(ServiceTasksTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, serviceTasks.init()); }

TEST_F(ServiceTasksTest, StartReturnsOk) { EXPECT_EQ(ESP_OK, serviceTasks.start()); }

TEST_F(ServiceTasksTest, ConstructorAddsLoggingAndOtaTaskStaticInfos) {
    ASSERT_EQ(2U, nodeStaticInfo.tasksStaticInfo.size());
    EXPECT_EQ(10, nodeStaticInfo.tasksStaticInfo[0].taskId);
    EXPECT_EQ("Logging Task", nodeStaticInfo.tasksStaticInfo[0].taskName);
    EXPECT_EQ(12, nodeStaticInfo.tasksStaticInfo[1].taskId);
    EXPECT_EQ("OTA Task", nodeStaticInfo.tasksStaticInfo[1].taskName);
}

TEST_F(ServiceTasksTest, GetLoggingTaskReturnsReference) {
    ILoggingTask &ref = serviceTasks.getLoggingTask();
    // Just verify the reference is valid (non-null)
    (void)ref;
    SUCCEED();
}

TEST_F(ServiceTasksTest, GetOtaTaskReturnsReference) {
    IOtaTask &ref = serviceTasks.getOtaTask();
    (void)ref;
    SUCCEED();
}

TEST_F(ServiceTasksTest, InitThenStartBothSucceed) {
    EXPECT_EQ(ESP_OK, serviceTasks.init());
    EXPECT_EQ(ESP_OK, serviceTasks.start());
}

TEST_F(ServiceTasksTest, StartReturnsFailWhenLoggingTaskStartFails) {
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("logging_task"), 4096, _, 5, nullptr))
        .WillOnce(Return(pdFAIL));
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("ota_task"), 8192, _, 8, nullptr)).Times(0);

    EXPECT_EQ(ESP_FAIL, serviceTasks.start());
}

TEST_F(ServiceTasksTest, StartReturnsFailWhenOtaTaskStartFails) {
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("logging_task"), 4096, _, 5, nullptr))
        .WillOnce(Return(pdPASS));
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("ota_task"), 8192, _, 8, nullptr))
        .WillOnce(Return(pdFAIL));

    EXPECT_EQ(ESP_FAIL, serviceTasks.start());
}

TEST(LoggingTaskTest, InitReturnsOkAndStartDelegatesToRtosTask) {
    ::testing::StrictMock<MockIFreeRtosFactory> mockRtosFactory;
    LoggingTask loggingTask(mockRtosFactory);

    EXPECT_EQ(ESP_OK, loggingTask.init());
    EXPECT_CALL(mockRtosFactory,
                createTask(_, StrEq("logging_task"), 4096, &loggingTask, 5, nullptr))
        .WillOnce(Return(pdPASS));
    EXPECT_EQ(ESP_OK, loggingTask.start());
}

TEST(LoggingTaskTest, TaskEntryReturnsImmediatelyWhenNoQueueIsPresent) {
    ::testing::StrictMock<MockIFreeRtosFactory> mockRtosFactory;
    ExposedLoggingTask loggingTask(mockRtosFactory);

    EXPECT_NO_THROW(loggingTask.callTaskEntry());
}

TEST(OtaTaskTest, InitReturnsOkAndStartDelegatesToRtosTask) {
    ::testing::StrictMock<MockIFreeRtosFactory> mockRtosFactory;
    OtaTask otaTask(mockRtosFactory);

    EXPECT_EQ(ESP_OK, otaTask.init());
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("ota_task"), 8192, &otaTask, 8, nullptr))
        .WillOnce(Return(pdPASS));
    EXPECT_EQ(ESP_OK, otaTask.start());
}

TEST(OtaTaskTest, TaskEntryReturnsImmediatelyWhenNoQueueIsPresent) {
    ::testing::StrictMock<MockIFreeRtosFactory> mockRtosFactory;
    ExposedOtaTask otaTask(mockRtosFactory);

    EXPECT_NO_THROW(otaTask.callTaskEntry());
}

class MasterServiceTasksTest : public ::testing::Test {
   protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    NodeStaticInfo nodeStaticInfo{1, "Node", "1.0.0", "1.0.0", "", ""};
    MasterServiceTasks serviceTasks{mockRtosFactory, nodeStaticInfo, 0};
};

TEST_F(MasterServiceTasksTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, serviceTasks.init()); }

TEST_F(MasterServiceTasksTest, StartReturnsOk) { EXPECT_EQ(ESP_OK, serviceTasks.start()); }

TEST_F(MasterServiceTasksTest, ConstructorAddsLoggingOtaAndStatusTaskStaticInfos) {
    ASSERT_EQ(3U, nodeStaticInfo.tasksStaticInfo.size());
    EXPECT_EQ(10, nodeStaticInfo.tasksStaticInfo[0].taskId);
    EXPECT_EQ("Logging Task", nodeStaticInfo.tasksStaticInfo[0].taskName);
    EXPECT_EQ(12, nodeStaticInfo.tasksStaticInfo[1].taskId);
    EXPECT_EQ("OTA Task", nodeStaticInfo.tasksStaticInfo[1].taskName);
    EXPECT_EQ(11, nodeStaticInfo.tasksStaticInfo[2].taskId);
    EXPECT_EQ("Status Task", nodeStaticInfo.tasksStaticInfo[2].taskName);
}

TEST_F(MasterServiceTasksTest, StartReturnsFailWhenStatusTaskStartFails) {
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("logging_task"), 4096, _, 5, nullptr))
        .WillOnce(Return(pdPASS));
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("ota_task"), 8192, _, 8, nullptr))
        .WillOnce(Return(pdPASS));
    EXPECT_CALL(mockRtosFactory, createTask(_, StrEq("status_task"), 4096, _, 5, nullptr))
        .WillOnce(Return(pdFAIL));

    EXPECT_EQ(ESP_FAIL, serviceTasks.start());
}

TEST_F(MasterServiceTasksTest, GetStatusTaskReturnsReference) {
    IStatusTask &ref = serviceTasks.getStatusTask();
    (void)ref;
    SUCCEED();
}
