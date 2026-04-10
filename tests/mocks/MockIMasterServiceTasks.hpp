#pragma once

#include <gmock/gmock.h>
#include "common/service_tasks/logging_task/ILoggingTask.hpp"
#include "common/service_tasks/status_task/IStatusTask.hpp"
#include "common/service_tasks/ota_task/IOtaTask.hpp"
#include "common/service_tasks/IMasterServiceTasks.hpp"

class StubMasterILoggingTask : public ILoggingTask {};
class StubMasterIStatusTask  : public IStatusTask  {};
class StubMasterIOtaTask     : public IOtaTask     {};

class MockIMasterServiceTasks : public IMasterServiceTasks
{
public:
    StubMasterILoggingTask stubLoggingTask;
    StubMasterIStatusTask  stubStatusTask;
    StubMasterIOtaTask     stubOtaTask;

    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));

    ILoggingTask &getLoggingTask() override { return stubLoggingTask; }
    IStatusTask  &getStatusTask()  override { return stubStatusTask;  }
    IOtaTask     &getOtaTask()     override { return stubOtaTask;     }
};
