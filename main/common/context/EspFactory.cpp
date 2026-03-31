#include "EspFactory.hpp"

EspFactory::EspFactory()
    : littleFs_()
    , httpServer_()
    , nvs_()
    , logger_()
    , freeRtosFactory_()
{
}

IEspLittleFs     &EspFactory::getLittleFs()          { return littleFs_;          }
IEspHttpServer   &EspFactory::getHttpServer()         { return httpServer_;        }
IEspNvs          &EspFactory::getNvs()                { return nvs_;               }
IEspLogger       &EspFactory::getLogger()             { return logger_;            }
IFreeRtosFactory &EspFactory::getFreeRtosFactory()    { return freeRtosFactory_;   }
