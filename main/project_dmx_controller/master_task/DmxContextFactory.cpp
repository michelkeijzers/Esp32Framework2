#include "DmxContextFactory.hpp"

DmxContextFactory::DmxContextFactory()
    : contextFactory_(),
      dmxApiFactory_(contextFactory_.getEspFactory().getHttpServer(),
                     contextFactory_.getEspFactory().getNvs()) {}

IContextFactory &DmxContextFactory::getContextFactory() { return contextFactory_; }
IDmxApiFactory &DmxContextFactory::getDmxApiFactory() { return dmxApiFactory_; }
