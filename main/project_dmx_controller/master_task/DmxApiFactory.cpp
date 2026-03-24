#include "DmxApiFactory.hpp"

DmxApiFactory::DmxApiFactory(IEspHttpServer &httpServer, IEspNvs &nvs)
    : presetManager_()
    , apiConfig_(httpServer, nvs)
    , apiPresets_(httpServer, presetManager_)
    , apiPresetValues_(httpServer, presetManager_)
    , apiConfiguration_(httpServer, nvs)
{
}

IApiConfig        &DmxApiFactory::getApiConfig()        { return apiConfig_;        }
IApiPresets       &DmxApiFactory::getApiPresets()       { return apiPresets_;       }
IApiPresetValues  &DmxApiFactory::getApiPresetValues()  { return apiPresetValues_;  }
IApiConfiguration &DmxApiFactory::getApiConfiguration() { return apiConfiguration_; }
