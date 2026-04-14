#include "EspNowFactory.hpp"

EspNowFactory::EspNowFactory() = default;

IEspNow &EspNowFactory::getEspNow() { return espNow_; }
