#pragma once
#include "IDmxApiFactory.hpp"
#include "../../presets_task/PresetManager.hpp"
#include "../webserver_task/apis/ApiConfig.hpp"
#include "../webserver_task/apis/ApiPresets.hpp"
#include "../webserver_task/apis/ApiPresetValues.hpp"
#include "../webserver_task/apis/ApiConfiguration.hpp"

class IEspHttpServer;
class IEspNvs;

class DmxApiFactory : public IDmxApiFactory
{
public:
    DmxApiFactory(IEspHttpServer &httpServer, IEspNvs &nvs);

    IApiConfig        &getApiConfig()        override;
    IApiPresets       &getApiPresets()       override;
    IApiPresetValues  &getApiPresetValues()  override;
    IApiConfiguration &getApiConfiguration() override;

private:
    PresetManager     presetManager_;
    ApiConfig         apiConfig_;
    ApiPresets        apiPresets_;
    ApiPresetValues   apiPresetValues_;
    ApiConfiguration  apiConfiguration_;
};
