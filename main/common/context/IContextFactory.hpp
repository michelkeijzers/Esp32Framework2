#pragma once

class IEspFactory;
class ICommonApiFactory;

class IContextFactory
{
public:
    virtual ~IContextFactory() = default;

    virtual IEspFactory       &getEspFactory()        = 0;
    virtual ICommonApiFactory &getCommonApiFactory()  = 0;
};
