#pragma once

class IApiStatus;
class IApiNodes;
class IApiSystem;
class IApiFirmware;
class IApiSecurity;
class IApiLogging;

class ICommonApiFactory
{
public:
    virtual ~ICommonApiFactory() = default;

    virtual IApiStatus   &getApiStatus()   = 0;
    virtual IApiNodes    &getApiNodes()    = 0;
    virtual IApiSystem   &getApiSystem()   = 0;
    virtual IApiFirmware &getApiFirmware() = 0;
    virtual IApiSecurity &getApiSecurity() = 0;
    virtual IApiLogging  &getApiLogging()  = 0;
};
