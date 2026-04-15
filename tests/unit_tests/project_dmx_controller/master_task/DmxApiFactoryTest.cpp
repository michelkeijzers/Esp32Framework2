#include <gtest/gtest.h>

#include "MockIEspHttpServer.hpp"
#include "MockIEspNvs.hpp"
#include "project_dmx_controller/master_task/DmxApiFactory.hpp"

TEST(DmxApiFactoryTest, GettersReturnStableReferences) {
    MockIEspHttpServer mockHttpServer;
    MockIEspNvs mockNvs;
    DmxApiFactory factory(mockHttpServer, mockNvs);

    EXPECT_EQ(&factory.getApiConfig(), &factory.getApiConfig());
    EXPECT_EQ(&factory.getApiPresets(), &factory.getApiPresets());
    EXPECT_EQ(&factory.getApiPresetValues(), &factory.getApiPresetValues());
    EXPECT_EQ(&factory.getApiConfiguration(), &factory.getApiConfiguration());
}
