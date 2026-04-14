#pragma once
#include "../web_server_task/apis/ApiFirmware.hpp"
#include "../web_server_task/apis/ApiLogging.hpp"
#include "../web_server_task/apis/ApiNodes.hpp"
#include "../web_server_task/apis/ApiSecurity.hpp"
#include "../web_server_task/apis/ApiStatus.hpp"
#include "../web_server_task/apis/ApiSystem.hpp"
#include "ICommonApiFactory.hpp"

class IEspHttpServer;

class CommonApiFactory : public ICommonApiFactory {
   public:
    explicit CommonApiFactory(IEspHttpServer &httpServer);

    IApiStatus &getApiStatus() override;
    IApiNodes &getApiNodes() override;
    IApiSystem &getApiSystem() override;
    IApiFirmware &getApiFirmware() override;
    IApiSecurity &getApiSecurity() override;
    IApiLogging &getApiLogging() override;

   private:
    ApiStatus apiStatus_;
    ApiNodes apiNodes_;
    ApiSystem apiSystem_;
    ApiFirmware apiFirmware_;
    ApiSecurity apiSecurity_;
    ApiLogging apiLogging_;
};
