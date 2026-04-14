#pragma once
#include "CommonApiFactory.hpp"
#include "EspFactory.hpp"
#include "IContextFactory.hpp"

class ContextFactory : public IContextFactory {
   public:
    ContextFactory();

    IEspFactory &getEspFactory() override;
    ICommonApiFactory &getCommonApiFactory() override;

   private:
    EspFactory espFactory_;
    CommonApiFactory commonApiFactory_;
};
