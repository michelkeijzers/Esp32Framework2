#pragma once
#include "project_dmx_controller/master_task/IDmxContextFactory.hpp"
#include "MockContextFactory.hpp"
#include "MockDmxApiFactory.hpp"

class MockDmxContextFactory : public IDmxContextFactory
{
public:
    MockContextFactory contextFactory_;
    MockDmxApiFactory  dmxApiFactory_;

    IContextFactory &getContextFactory() override { return contextFactory_; }
    IDmxApiFactory  &getDmxApiFactory()  override { return dmxApiFactory_;  }
};
