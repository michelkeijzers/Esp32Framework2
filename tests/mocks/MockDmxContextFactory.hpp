#pragma once
#include "MockContextFactory.hpp"
#include "MockDmxApiFactory.hpp"
#include "project_dmx_controller/master_task/IDmxContextFactory.hpp"

class MockDmxContextFactory : public IDmxContextFactory {
   public:
    MockContextFactory contextFactory_;
    MockDmxApiFactory dmxApiFactory_;

    IContextFactory &getContextFactory() override { return contextFactory_; }
    IDmxApiFactory &getDmxApiFactory() override { return dmxApiFactory_; }
};
