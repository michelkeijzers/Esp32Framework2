#pragma once
#include <gmock/gmock.h>
#include "project_dmx_controller/master_task/IDmxApiFactory.hpp"
#include "MockIApiConfig.hpp"
#include "MockIApiPresets.hpp"
#include "MockIApiPresetValues.hpp"
#include "MockIApiConfiguration.hpp"

class MockDmxApiFactory : public IDmxApiFactory
{
public:
    ::testing::NiceMock<MockIApiConfig>        apiConfig_;
    ::testing::NiceMock<MockIApiPresets>       apiPresets_;
    ::testing::NiceMock<MockIApiPresetValues>  apiPresetValues_;
    ::testing::NiceMock<MockIApiConfiguration> apiConfiguration_;

    IApiConfig        &getApiConfig()        override { return apiConfig_;        }
    IApiPresets       &getApiPresets()       override { return apiPresets_;       }
    IApiPresetValues  &getApiPresetValues()  override { return apiPresetValues_;  }
    IApiConfiguration &getApiConfiguration() override { return apiConfiguration_; }
};
