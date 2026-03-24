#pragma once
#include <gmock/gmock.h>
#include "common/context/ICommonApiFactory.hpp"
#include "MockIApiStatus.hpp"
#include "MockIApiNodes.hpp"
#include "MockIApiSystem.hpp"
#include "MockIApiFirmware.hpp"
#include "MockIApiSecurity.hpp"
#include "MockIApiLogging.hpp"

class MockCommonApiFactory : public ICommonApiFactory
{
public:
    ::testing::NiceMock<MockIApiStatus>   apiStatus_;
    ::testing::NiceMock<MockIApiNodes>    apiNodes_;
    ::testing::NiceMock<MockIApiSystem>   apiSystem_;
    ::testing::NiceMock<MockIApiFirmware> apiFirmware_;
    ::testing::NiceMock<MockIApiSecurity> apiSecurity_;
    ::testing::NiceMock<MockIApiLogging>  apiLogging_;

    IApiStatus   &getApiStatus()   override { return apiStatus_;   }
    IApiNodes    &getApiNodes()    override { return apiNodes_;    }
    IApiSystem   &getApiSystem()   override { return apiSystem_;   }
    IApiFirmware &getApiFirmware() override { return apiFirmware_; }
    IApiSecurity &getApiSecurity() override { return apiSecurity_; }
    IApiLogging  &getApiLogging()  override { return apiLogging_;  }
};
