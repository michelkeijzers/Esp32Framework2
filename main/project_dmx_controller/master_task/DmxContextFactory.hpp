#pragma once
#include "../../../common/context/ContextFactory.hpp"
#include "DmxApiFactory.hpp"
#include "IDmxContextFactory.hpp"

class DmxContextFactory : public IDmxContextFactory {
   public:
    DmxContextFactory();

    IContextFactory &getContextFactory() override;
    IDmxApiFactory &getDmxApiFactory() override;

   private:
    ContextFactory contextFactory_;
    DmxApiFactory dmxApiFactory_;
};
