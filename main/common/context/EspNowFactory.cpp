#include "EspNowFactory.hpp"

EspNowFactory::EspNowFactory()
    : espNow_()
{
}

IEspNow &EspNowFactory::getEspNow() { return espNow_; }
