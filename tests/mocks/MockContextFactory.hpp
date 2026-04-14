#pragma once
#include "MockCommonApiFactory.hpp"
#include "MockEspFactory.hpp"
#include "common/context/IContextFactory.hpp"

class MockContextFactory : public IContextFactory {
   public:
    MockEspFactory espFactory_;
    MockCommonApiFactory commonApiFactory_;

    IEspFactory &getEspFactory() override { return espFactory_; }
    ICommonApiFactory &getCommonApiFactory() override { return commonApiFactory_; }
};
