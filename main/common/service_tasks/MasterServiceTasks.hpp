#pragma once

#include "IMasterServiceTasks.hpp"
#include "ServiceTasks.hpp"
#include "status_task/StatusTask.hpp"

class IFreeRtosFactory;
class NodeStaticInfo;

/**
 * Concrete container for master service tasks.
 *
 * Extends common service tasks with StatusTask.
 */
class MasterServiceTasks : public IMasterServiceTasks {
   public:
    MasterServiceTasks(IFreeRtosFactory &freeRtosFactory, NodeStaticInfo &nodeStaticInfo,
                       int nodeOffset);
    ~MasterServiceTasks() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

    ILoggingTask &getLoggingTask() override;
    IOtaTask &getOtaTask() override;
    IStatusTask &getStatusTask() override;

   private:
    ServiceTasks serviceTasks_;
    StatusTask statusTask_;
};
