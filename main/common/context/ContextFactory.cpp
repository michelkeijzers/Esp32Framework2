#include "ContextFactory.hpp"

ContextFactory::ContextFactory()
    : espFactory_()
    , commonApiFactory_(espFactory_.getHttpServer())
{
}

IEspFactory       &ContextFactory::getEspFactory()       { return espFactory_;        }
ICommonApiFactory &ContextFactory::getCommonApiFactory() { return commonApiFactory_;  }
