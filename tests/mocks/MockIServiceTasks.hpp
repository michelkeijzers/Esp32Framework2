#pragma once
#include <gmock/gmock.h>
#include "common/service_tasks/logging_task/ILoggingTask.hpp"
#include "common/service_tasks/ota_task/IOtaTask.hpp"
#include "common/service_tasks/ISlaveServiceTasks.hpp"

// Minimal concrete stubs for empty task interfaces
class StubILoggingTask : public ILoggingTask
{
};
class StubIOtaTask     : public IOtaTask     {};

class MockIServiceTasks : public ISlaveServiceTasks
{
public:
    StubILoggingTask stubLoggingTask;
    StubIOtaTask     stubOtaTask;

    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));

    ILoggingTask &getLoggingTask() override { return stubLoggingTask; }
    IOtaTask     &getOtaTask()     override { return stubOtaTask;     }
};
