#pragma once
#include "ICommonApiFactory.hpp"
#include "../webserver_task/apis/ApiStatus.hpp"
#include "../webserver_task/apis/ApiNodes.hpp"
#include "../webserver_task/apis/ApiSystem.hpp"
#include "../webserver_task/apis/ApiFirmware.hpp"
#include "../webserver_task/apis/ApiSecurity.hpp"
#include "../webserver_task/apis/ApiLogging.hpp"

class IEspHttpServer;

class CommonApiFactory : public ICommonApiFactory
{
public:
    explicit CommonApiFactory(IEspHttpServer &httpServer);

    IApiStatus   &getApiStatus()   override;
    IApiNodes    &getApiNodes()    override;
    IApiSystem   &getApiSystem()   override;
    IApiFirmware &getApiFirmware() override;
    IApiSecurity &getApiSecurity() override;
    IApiLogging  &getApiLogging()  override;

private:
    ApiStatus   apiStatus_;
    ApiNodes    apiNodes_;
    ApiSystem   apiSystem_;
    ApiFirmware apiFirmware_;
    ApiSecurity apiSecurity_;
    ApiLogging  apiLogging_;
};
