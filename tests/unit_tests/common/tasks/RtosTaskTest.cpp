#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockIFreeRtosFactory.hpp"
#include "common/tasks/RtosTask.hpp"

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

class TestableRtosTask : public RtosTask {
   public:
    TestableRtosTask(IFreeRtosFactory& freeRtosFactory, const char* name, uint32_t stackSize,
                     UBaseType_t priority)
        : RtosTask(freeRtosFactory, name, stackSize, priority) {}

    using RtosTask::queues_;

    void callTaskEntry() { taskEntry(); }
};

class RtosTaskTest : public ::testing::Test {
   protected:
    ::testing::StrictMock<MockIFreeRtosFactory> mockFreeRtosFactory;
    TestableRtosTask task{mockFreeRtosFactory, "test_task", 1024, 3};
};

TEST_F(RtosTaskTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, task.init()); }

TEST_F(RtosTaskTest, StartReturnsFailWhenTaskCreationFails) {
    EXPECT_CALL(mockFreeRtosFactory, createTask(_, StrEq("test_task"), 1024, &task,
                                                static_cast<UBaseType_t>(3), nullptr))
        .WillOnce(Return(pdFAIL));

    EXPECT_EQ(ESP_FAIL, task.start());
    EXPECT_TRUE(task.queues_.empty());
}

TEST_F(RtosTaskTest, StartCreatesDefaultQueueWhenTaskCreationSucceeds) {
    EXPECT_CALL(mockFreeRtosFactory, createTask(_, StrEq("test_task"), 1024, &task,
                                                static_cast<UBaseType_t>(3), nullptr))
        .WillOnce(Return(pdPASS));

    EXPECT_EQ(ESP_OK, task.start());
    ASSERT_EQ(1U, task.queues_.size());
    EXPECT_EQ(sizeof(int), task.queues_.front()->itemSize());
    EXPECT_EQ(10U, task.queues_.front()->capacity());
}

TEST_F(RtosTaskTest, CreateQueueStoresQueueAndReturnsPointer) {
    IRtosQueue* queue = task.createQueue(sizeof(uint16_t), 4);

    ASSERT_NE(nullptr, queue);
    ASSERT_EQ(1U, task.queues_.size());
    EXPECT_EQ(sizeof(uint16_t), queue->itemSize());
    EXPECT_EQ(4U, queue->capacity());
}

TEST_F(RtosTaskTest, GetTickCountForwardsToFactory) {
    EXPECT_CALL(mockFreeRtosFactory, getTickCount()).WillOnce(Return(1234));

    EXPECT_EQ(static_cast<TickType_t>(1234), task.getTickCount());
}

TEST_F(RtosTaskTest, TaskEntryIsNoOpByDefault) { EXPECT_NO_THROW(task.callTaskEntry()); }
