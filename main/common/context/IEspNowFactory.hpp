#pragma once

class IEspNow;

class IEspNowFactory
{
public:
    virtual ~IEspNowFactory() = default;

    virtual IEspNow &getEspNow() = 0;
};
