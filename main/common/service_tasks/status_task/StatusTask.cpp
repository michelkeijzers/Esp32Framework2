#include "StatusTask.hpp"

#include <algorithm>
#include <cstring>

#include "free_rtos_if.hpp"

static constexpr const char *STATUS_TASK_NAME = "status_task";
static constexpr uint32_t STATUS_TASK_STACK_SIZE = 4096;
static constexpr UBaseType_t STATUS_TASK_PRIORITY = 5;
static constexpr uint32_t HEARTBEAT_CHECK_INTERVAL_MS = 5000;
static constexpr uint32_t HEARTBEAT_TIMEOUT_MS = 5000;

StatusTask::StatusTask(IFreeRtosFactory &freeRtosFactory)
    : RtosTask(freeRtosFactory, STATUS_TASK_NAME, STATUS_TASK_STACK_SIZE, STATUS_TASK_PRIORITY) {}

esp_err_t StatusTask::init() { return RtosTask::init(); }

esp_err_t StatusTask::start() { return RtosTask::start(); }

void StatusTask::taskEntry() {
    if (queues_.empty()) {
        return;
    }

    IRtosQueue *queue = queues_.front().get();
    StatusTaskMessages::MessageEnvelope msg{};

    TickType_t lastCheckTick = getTickCount();
    const TickType_t checkIntervalTicks = pdMS_TO_TICKS(HEARTBEAT_CHECK_INTERVAL_MS);

    while (true) {
        if (queue && queue->receive(&msg, pdMS_TO_TICKS(100))) {
            handleMessage(msg);
        }

        const TickType_t now = getTickCount();
        if ((now - lastCheckTick) >= checkIntervalTicks) {
            handleHeartbeatTimer(now);
            lastCheckTick = now;
        }
    }
}

void StatusTask::handleMessage(const StatusTaskMessages::MessageEnvelope &msg) {
    switch (msg.type) {
        case StatusTaskMessages::Type::SET_TASK_NAME: {
            const uint32_t taskId = static_cast<uint32_t>(msg.data.setTaskName.task_id);
            auto it = std::find_if(taskData.begin(), taskData.end(),
                                   [&](const TaskData &t) { return t.getId() == taskId; });

            if (it == taskData.end()) {
                taskData.emplace_back(taskId, msg.data.setTaskName.task_name, getTickCount());
            } else {
                // Same task registered again; refresh heartbeat timestamp.
                it->updateHeartbeat(getTickCount());
            }
            break;
        }
        case StatusTaskMessages::Type::TASK_HEARTBEAT: {
            const uint32_t taskId = static_cast<uint32_t>(msg.data.taskHeartbeat.task_id);
            auto it = std::find_if(taskData.begin(), taskData.end(),
                                   [&](const TaskData &t) { return t.getId() == taskId; });

            if (it != taskData.end()) {
                it->updateHeartbeat(getTickCount());
            }
            break;
        }
        default:
            break;
    }
}

void StatusTask::handleHeartbeatTimer(TickType_t now) {
    const TickType_t timeoutTicks = pdMS_TO_TICKS(HEARTBEAT_TIMEOUT_MS);
    bool allHeartbeatsOk = !taskData.empty();

    for (const auto &task : taskData) {
        if ((now - task.getLastHeartbeatTick()) >= timeoutTicks) {
            allHeartbeatsOk = false;

            StatusTaskMessages::MessageEnvelope timeoutMsg{};
            timeoutMsg.type = StatusTaskMessages::Type::HEARTBEAT_TIMEOUT;
            timeoutMsg.data.heartbeatTimeout.task_id = static_cast<int32_t>(task.getId());
            std::strncpy(timeoutMsg.data.heartbeatTimeout.task_name, task.getName(),
                         StatusTaskMessages::TASK_NAME_MAX_LEN);
            timeoutMsg.data.heartbeatTimeout.task_name[StatusTaskMessages::TASK_NAME_MAX_LEN - 1] =
                '\0';
            // TODO: send timeoutMsg to Web Server Task queue.
        }
    }

    if (allHeartbeatsOk) {
        // TODO: send ALL_HEARTBEATS_OK to Web Server Task queue.
    }
}
