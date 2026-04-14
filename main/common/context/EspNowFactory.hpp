#pragma once
#include "../esp/esp_now/EspNow.hpp"
#include "IEspNowFactory.hpp"

class EspNowFactory : public IEspNowFactory {
   public:
    EspNowFactory();

    IEspNow &getEspNow() override;

   private:
    EspNow espNow_;
};
