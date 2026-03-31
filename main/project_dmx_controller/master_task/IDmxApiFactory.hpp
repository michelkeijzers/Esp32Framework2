#pragma once

class IApiConfig;
class IApiPresets;
class IApiPresetValues;
class IApiConfiguration;

class IDmxApiFactory
{
public:
    virtual ~IDmxApiFactory() = default;

    virtual IApiConfig        &getApiConfig()        = 0;
    virtual IApiPresets       &getApiPresets()       = 0;
    virtual IApiPresetValues  &getApiPresetValues()  = 0;
    virtual IApiConfiguration &getApiConfiguration() = 0;
};
