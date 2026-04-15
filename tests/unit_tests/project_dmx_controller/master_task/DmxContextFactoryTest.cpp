#include <gtest/gtest.h>

#include "project_dmx_controller/master_task/DmxContextFactory.hpp"

TEST(DmxContextFactoryTest, GettersReturnStableReferences) {
    DmxContextFactory factory;

    EXPECT_EQ(&factory.getContextFactory(), &factory.getContextFactory());
    EXPECT_EQ(&factory.getDmxApiFactory(), &factory.getDmxApiFactory());
}
