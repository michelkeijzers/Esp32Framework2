#include "ContextFactory.hpp"

ContextFactory::ContextFactory() : commonApiFactory_(espFactory_.getHttpServer()) {}

IEspFactory& ContextFactory::getEspFactory() { return espFactory_; }
ICommonApiFactory& ContextFactory::getCommonApiFactory() { return commonApiFactory_; }
