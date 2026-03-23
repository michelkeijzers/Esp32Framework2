#pragma once
#include <gmock/gmock.h>
#include "common/service_tasks/logging_task/ILoggingTask.hpp"
#include "common/service_tasks/status_task/IStatusTask.hpp"
#include "common/service_tasks/ota_task/IOtaTask.hpp"
#include "common/service_tasks/IServiceTasks.hpp"

// Minimal concrete stubs for empty task interfaces
class StubILoggingTask : public ILoggingTask {};
class StubIStatusTask  : public IStatusTask  {};
class StubIOtaTask     : public IOtaTask     {};

class MockIServiceTasks : public IServiceTasks
{
public:
    StubILoggingTask stubLoggingTask;
    StubIStatusTask  stubStatusTask;
    StubIOtaTask     stubOtaTask;

    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));

    ILoggingTask &getLoggingTask() override { return stubLoggingTask; }
    IStatusTask  &getStatusTask()  override { return stubStatusTask;  }
    IOtaTask     &getOtaTask()     override { return stubOtaTask;     }
};
