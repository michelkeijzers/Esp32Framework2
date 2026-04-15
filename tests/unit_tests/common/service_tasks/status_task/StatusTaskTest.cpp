#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/tasks/RtosQueue.hpp"

#define protected public
#define private public
#include "common/service_tasks/status_task/StatusTask.hpp"
#undef private
#undef protected

#include "MockIFreeRtosFactory.hpp"

using ::testing::Return;

namespace {

class StopTaskLoopException final : public std::runtime_error {
   public:
    StopTaskLoopException() : std::runtime_error("stop task loop") {}
};

class ControlledReceiveQueue final : public RtosQueue {
   public:
    explicit ControlledReceiveQueue(std::vector<StatusTaskMessages::MessageEnvelope> queuedMessages,
                                    bool keepReturningFalseAfterMessages = true)
        : RtosQueue(sizeof(StatusTaskMessages::MessageEnvelope), 8),
          queuedMessages_(std::move(queuedMessages)),
          keepReturningFalseAfterMessages_(keepReturningFalseAfterMessages) {}

    bool receive(void* item, uint32_t timeoutMs) override {
        (void)timeoutMs;
        if (readIndex_ < queuedMessages_.size()) {
            std::memcpy(item, &queuedMessages_[readIndex_],
                        sizeof(StatusTaskMessages::MessageEnvelope));
            ++readIndex_;
            return true;
        }
        return keepReturningFalseAfterMessages_;
    }

   private:
    std::vector<StatusTaskMessages::MessageEnvelope> queuedMessages_;
    std::size_t readIndex_{0};
    bool keepReturningFalseAfterMessages_{true};
};

StatusTaskMessages::MessageEnvelope makeSetTaskNameMessage(int32_t taskId, const char* taskName) {
    StatusTaskMessages::MessageEnvelope msg{};
    msg.type = StatusTaskMessages::Type::SET_TASK_NAME;
    msg.data.setTaskName.task_id = taskId;
    std::strncpy(msg.data.setTaskName.task_name, taskName, StatusTaskMessages::TASK_NAME_MAX_LEN);
    msg.data.setTaskName.task_name[StatusTaskMessages::TASK_NAME_MAX_LEN - 1] = '\0';
    return msg;
}

StatusTaskMessages::MessageEnvelope makeHeartbeatMessage(int32_t taskId) {
    StatusTaskMessages::MessageEnvelope msg{};
    msg.type = StatusTaskMessages::Type::TASK_HEARTBEAT;
    msg.data.taskHeartbeat.task_id = taskId;
    return msg;
}

StatusTaskMessages::MessageEnvelope makeAllHeartbeatsOkMessage() {
    StatusTaskMessages::MessageEnvelope msg{};
    msg.type = StatusTaskMessages::Type::ALL_HEARTBEATS_OK;
    return msg;
}

}  // namespace

class StatusTaskTest : public ::testing::Test {
   protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    StatusTask statusTask{mockRtosFactory};
};

TEST_F(StatusTaskTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, statusTask.init()); }

TEST_F(StatusTaskTest, StartReturnsOk) { EXPECT_EQ(ESP_OK, statusTask.start()); }

TEST_F(StatusTaskTest, HandleSetTaskNameAddsNewTaskData) {
    const auto msg = makeSetTaskNameMessage(7, "worker-A");

    EXPECT_CALL(mockRtosFactory, getTickCount()).WillOnce(Return(120));

    statusTask.handleMessage(msg);

    ASSERT_EQ(1U, statusTask.taskData.size());
    EXPECT_EQ(7U, statusTask.taskData[0].getId());
    EXPECT_STREQ("worker-A", statusTask.taskData[0].getName());
    EXPECT_EQ(120U, statusTask.taskData[0].getLastHeartbeatTick());
}

TEST_F(StatusTaskTest, HandleSetTaskNameForExistingTaskRefreshesHeartbeatWithoutDuplicate) {
    const auto firstMsg = makeSetTaskNameMessage(11, "first-name");
    const auto secondMsg = makeSetTaskNameMessage(11, "second-name");

    EXPECT_CALL(mockRtosFactory, getTickCount()).WillOnce(Return(10)).WillOnce(Return(42));

    statusTask.handleMessage(firstMsg);
    statusTask.handleMessage(secondMsg);

    ASSERT_EQ(1U, statusTask.taskData.size());
    EXPECT_EQ(11U, statusTask.taskData[0].getId());
    EXPECT_STREQ("first-name", statusTask.taskData[0].getName());
    EXPECT_EQ(42U, statusTask.taskData[0].getLastHeartbeatTick());
}

TEST_F(StatusTaskTest, HandleTaskHeartbeatUpdatesExistingTaskHeartbeat) {
    const auto setNameMsg = makeSetTaskNameMessage(3, "heartbeat-task");
    const auto heartbeatMsg = makeHeartbeatMessage(3);

    EXPECT_CALL(mockRtosFactory, getTickCount()).WillOnce(Return(5)).WillOnce(Return(77));

    statusTask.handleMessage(setNameMsg);
    statusTask.handleMessage(heartbeatMsg);

    ASSERT_EQ(1U, statusTask.taskData.size());
    EXPECT_EQ(77U, statusTask.taskData[0].getLastHeartbeatTick());
}

TEST_F(StatusTaskTest, HandleTaskHeartbeatForUnknownTaskDoesNothing) {
    const auto heartbeatMsg = makeHeartbeatMessage(1234);

    EXPECT_CALL(mockRtosFactory, getTickCount()).Times(0);

    statusTask.handleMessage(heartbeatMsg);

    EXPECT_TRUE(statusTask.taskData.empty());
}

TEST_F(StatusTaskTest, HandleMessageDefaultBranchDoesNothing) {
    const auto unknownMsg = makeAllHeartbeatsOkMessage();

    EXPECT_CALL(mockRtosFactory, getTickCount()).Times(0);

    statusTask.handleMessage(unknownMsg);

    EXPECT_TRUE(statusTask.taskData.empty());
}

TEST_F(StatusTaskTest, HandleHeartbeatTimerWithNoTaskDataDoesNotCrash) {
    EXPECT_TRUE(statusTask.taskData.empty());

    statusTask.handleHeartbeatTimer(1000);

    EXPECT_TRUE(statusTask.taskData.empty());
}

TEST_F(StatusTaskTest, HandleHeartbeatTimerAllHeartbeatsOkPathWithRecentHeartbeat) {
    statusTask.taskData.emplace_back(9U, "still-alive", 50U);

    statusTask.handleHeartbeatTimer(549U);

    ASSERT_EQ(1U, statusTask.taskData.size());
    EXPECT_EQ(9U, statusTask.taskData[0].getId());
    EXPECT_STREQ("still-alive", statusTask.taskData[0].getName());
}

TEST_F(StatusTaskTest, HandleHeartbeatTimerTimeoutPathExecutesForStaleTaskName) {
    const std::string veryLongTaskName(StatusTaskMessages::TASK_NAME_MAX_LEN + 25U, 'X');
    statusTask.taskData.emplace_back(4U, veryLongTaskName.c_str(), 0U);

    statusTask.handleHeartbeatTimer(600U);

    ASSERT_EQ(1U, statusTask.taskData.size());
    EXPECT_EQ(4U, statusTask.taskData[0].getId());
}

TEST_F(StatusTaskTest, TaskEntryReturnsImmediatelyWhenNoQueuesAreAvailable) {
    ASSERT_TRUE(statusTask.queues_.empty());

    EXPECT_NO_THROW(statusTask.taskEntry());
}

TEST_F(StatusTaskTest, TaskEntryHandlesReceiveFalseAndPeriodicHeartbeatCheckThenExits) {
    statusTask.queues_.push_back(std::make_unique<ControlledReceiveQueue>(
        std::vector<StatusTaskMessages::MessageEnvelope>{}, true));

    EXPECT_CALL(mockRtosFactory, getTickCount())
        .WillOnce(Return(0U))
        .WillOnce(Return(pdMS_TO_TICKS(5000U) + 1U))
        .WillOnce(::testing::Throw(StopTaskLoopException{}));

    EXPECT_THROW(statusTask.taskEntry(), StopTaskLoopException);
}

TEST_F(StatusTaskTest, TaskEntryReceivesMessageAndDispatchesToHandleMessage) {
    std::vector<StatusTaskMessages::MessageEnvelope> queuedMessages;
    queuedMessages.push_back(makeSetTaskNameMessage(88, "from-queue"));
    statusTask.queues_.push_back(
        std::make_unique<ControlledReceiveQueue>(std::move(queuedMessages), true));

    EXPECT_CALL(mockRtosFactory, getTickCount())
        .WillOnce(Return(0U))
        .WillOnce(Return(33U))
        .WillOnce(::testing::Throw(StopTaskLoopException{}));

    EXPECT_THROW(statusTask.taskEntry(), StopTaskLoopException);

    ASSERT_EQ(1U, statusTask.taskData.size());
    EXPECT_EQ(88U, statusTask.taskData[0].getId());
    EXPECT_STREQ("from-queue", statusTask.taskData[0].getName());
    EXPECT_EQ(33U, statusTask.taskData[0].getLastHeartbeatTick());
}
