#pragma once
#include <gmock/gmock.h>
#include "common/context/IEspFactory.hpp"
#include "MockIEspLittleFs.hpp"
#include "MockIEspHttpServer.hpp"
#include "MockIEspNvs.hpp"
#include "MockIEspLogger.hpp"
#include "MockIFreeRtosFactory.hpp"

class MockEspFactory : public IEspFactory
{
public:
    ::testing::NiceMock<MockIEspLittleFs>       littleFs_;
    ::testing::NiceMock<MockIEspHttpServer>     httpServer_;
    ::testing::NiceMock<MockIEspNvs>            nvs_;
    ::testing::NiceMock<MockIEspLogger>         logger_;
    ::testing::NiceMock<MockIFreeRtosFactory>   freeRtosFactory_;

    IEspLittleFs     &getLittleFs()          override { return littleFs_;          }
    IEspHttpServer   &getHttpServer()         override { return httpServer_;        }
    IEspNvs          &getNvs()                override { return nvs_;               }
    IEspLogger       &getLogger()             override { return logger_;            }
    IFreeRtosFactory &getFreeRtosFactory()    override { return freeRtosFactory_;   }
};
