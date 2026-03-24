#pragma once

class IEspLittleFs;
class IEspHttpServer;
class IEspNvs;
class IEspLogger;

class IEspFactory
{
public:
    virtual ~IEspFactory() = default;

    virtual IEspLittleFs  &getLittleFs()  = 0;
    virtual IEspHttpServer &getHttpServer() = 0;
    virtual IEspNvs        &getNvs()       = 0;
    virtual IEspLogger     &getLogger()    = 0;
};
