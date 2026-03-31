#pragma once
#include "common/context/IContextFactory.hpp"
#include "MockEspFactory.hpp"
#include "MockCommonApiFactory.hpp"

class MockContextFactory : public IContextFactory
{
public:
    MockEspFactory       espFactory_;
    MockCommonApiFactory commonApiFactory_;

    IEspFactory       &getEspFactory()       override { return espFactory_;       }
    ICommonApiFactory &getCommonApiFactory() override { return commonApiFactory_; }
};
