#pragma once
#include "../../presets_task/PresetManager.hpp"
#include "../web_server_task/apis/ApiConfig.hpp"
#include "../web_server_task/apis/ApiConfiguration.hpp"
#include "../web_server_task/apis/ApiPresetValues.hpp"
#include "../web_server_task/apis/ApiPresets.hpp"
#include "IDmxApiFactory.hpp"

class IEspHttpServer;
class IEspNvs;

class DmxApiFactory : public IDmxApiFactory {
   public:
    DmxApiFactory(IEspHttpServer &httpServer, IEspNvs &nvs);

    IApiConfig &getApiConfig() override;
    IApiPresets &getApiPresets() override;
    IApiPresetValues &getApiPresetValues() override;
    IApiConfiguration &getApiConfiguration() override;

   private:
    PresetManager presetManager_;
    ApiConfig apiConfig_;
    ApiPresets apiPresets_;
    ApiPresetValues apiPresetValues_;
    ApiConfiguration apiConfiguration_;
};
