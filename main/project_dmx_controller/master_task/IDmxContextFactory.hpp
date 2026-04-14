#pragma once
#include "../../../common/context/IContextFactory.hpp"

class IDmxApiFactory;

class IDmxContextFactory {
   public:
    virtual ~IDmxContextFactory() = default;

    virtual IContextFactory &getContextFactory() = 0;
    virtual IDmxApiFactory &getDmxApiFactory() = 0;
};
