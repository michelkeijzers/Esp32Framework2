#pragma once
#include "IContextFactory.hpp"
#include "EspFactory.hpp"
#include "CommonApiFactory.hpp"

class ContextFactory : public IContextFactory
{
public:
    ContextFactory();

    IEspFactory       &getEspFactory()       override;
    ICommonApiFactory &getCommonApiFactory() override;

private:
    EspFactory        espFactory_;
    CommonApiFactory  commonApiFactory_;
};
