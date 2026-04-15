#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include "common/service_tasks/status_task/TaskData.hpp"

namespace {

constexpr std::size_t kTaskNameBufferLength = 32U;

}  // namespace

TEST(TaskDataTest, ConstructorStoresIdNameAndHeartbeatTick) {
    const uint32_t taskId = 42U;
    const TickType_t heartbeatTick = 123U;
    const char* taskName = "worker-main";

    TaskData taskData(taskId, taskName, heartbeatTick);

    EXPECT_EQ(taskId, taskData.getId());
    EXPECT_STREQ(taskName, taskData.getName());
    EXPECT_EQ(heartbeatTick, taskData.getLastHeartbeatTick());
}

TEST(TaskDataTest, UpdateHeartbeatReplacesStoredTick) {
    TaskData taskData(1U, "task", 10U);

    taskData.updateHeartbeat(77U);

    EXPECT_EQ(77U, taskData.getLastHeartbeatTick());
}

TEST(TaskDataTest, ConstructorTruncatesNameLongerThanBufferAndNullTerminates) {
    const std::string longName(kTaskNameBufferLength + 10U, 'X');

    TaskData taskData(5U, longName.c_str(), 0U);

    const std::string storedName(taskData.getName());
    EXPECT_EQ(kTaskNameBufferLength - 1U, storedName.size());
    EXPECT_EQ(std::string(kTaskNameBufferLength - 1U, 'X'), storedName);
}

TEST(TaskDataTest, ConstructorKeepsNameWithExactlyMaxMinusOneChars) {
    const std::string exactFitName(kTaskNameBufferLength - 1U, 'A');

    TaskData taskData(6U, exactFitName.c_str(), 50U);

    EXPECT_EQ(exactFitName, std::string(taskData.getName()));
}

TEST(TaskDataTest, ConstructorHandlesEmptyName) {
    TaskData taskData(7U, "", 9U);

    EXPECT_STREQ("", taskData.getName());
    EXPECT_EQ(9U, taskData.getLastHeartbeatTick());
}

TEST(TaskDataTest, ConstructorPreservesEmbeddedWhitespaceInName) {
    const char* taskName = "status task monitor";

    TaskData taskData(8U, taskName, 21U);

    EXPECT_STREQ(taskName, taskData.getName());
}

TEST(TaskDataTest, MultipleHeartbeatUpdatesKeepLatestValue) {
    TaskData taskData(9U, "multi-update", 1U);

    taskData.updateHeartbeat(2U);
    taskData.updateHeartbeat(3U);
    taskData.updateHeartbeat(99U);

    EXPECT_EQ(99U, taskData.getLastHeartbeatTick());
}

TEST(TaskDataTest, DifferentInstancesMaintainIndependentState) {
    TaskData taskA(100U, "alpha", 10U);
    TaskData taskB(200U, "beta", 20U);

    taskA.updateHeartbeat(55U);

    EXPECT_EQ(100U, taskA.getId());
    EXPECT_STREQ("alpha", taskA.getName());
    EXPECT_EQ(55U, taskA.getLastHeartbeatTick());

    EXPECT_EQ(200U, taskB.getId());
    EXPECT_STREQ("beta", taskB.getName());
    EXPECT_EQ(20U, taskB.getLastHeartbeatTick());
}
