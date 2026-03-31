#include <gtest/gtest.h>

#include "../../../mocks/MockDmxApiFactory.hpp"
#include "../../../mocks/MockEspNowFactory.hpp"

TEST(MockDmxApiFactoryTest, ReturnsStableApiReferences)
{
    MockDmxApiFactory factory;

    EXPECT_EQ(&factory.getApiConfig(), &factory.getApiConfig());
    EXPECT_EQ(&factory.getApiPresets(), &factory.getApiPresets());
    EXPECT_EQ(&factory.getApiPresetValues(), &factory.getApiPresetValues());
    EXPECT_EQ(&factory.getApiConfiguration(), &factory.getApiConfiguration());
}

TEST(MockEspNowFactoryTest, ReturnsStableEspNowReference)
{
    MockEspNowFactory factory;

    EXPECT_EQ(&factory.getEspNow(), &factory.getEspNow());
}
