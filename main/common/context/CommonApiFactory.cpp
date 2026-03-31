#include "CommonApiFactory.hpp"

CommonApiFactory::CommonApiFactory(IEspHttpServer &httpServer)
    : apiStatus_(httpServer)
    , apiNodes_(httpServer)
    , apiSystem_(httpServer)
    , apiFirmware_(httpServer)
    , apiSecurity_(httpServer)
    , apiLogging_(httpServer)
{
}

IApiStatus        &CommonApiFactory::getApiStatus()          { return apiStatus_;          }
IApiNodes         &CommonApiFactory::getApiNodes()           { return apiNodes_;           }
IApiSystem        &CommonApiFactory::getApiSystem()          { return apiSystem_;          }
IApiFirmware      &CommonApiFactory::getApiFirmware()        { return apiFirmware_;        }
IApiSecurity      &CommonApiFactory::getApiSecurity()        { return apiSecurity_;        }
IApiLogging       &CommonApiFactory::getApiLogging()         { return apiLogging_;         }
