#pragma once
#include "IEspNowFactory.hpp"
#include "../esp/esp_now/EspNow.hpp"

class EspNowFactory : public IEspNowFactory
{
public:
    EspNowFactory();

    IEspNow &getEspNow() override;

private:
    EspNow espNow_;
};
