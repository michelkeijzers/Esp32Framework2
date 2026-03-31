#pragma once
#include "IEspFactory.hpp"
#include "../esp/esp_file_systems/EspLittleFs.hpp"
#include "../esp/esp_http_server/EspHttpServer.hpp"
#include "../esp/esp_nvs/EspNvs.hpp"
#include "../esp/esp_logger/EspLogger.hpp"
#include "../esp/free_rtos/FreeRtosFactory.hpp"

class EspFactory : public IEspFactory
{
public:
    EspFactory();

    IEspLittleFs     &getLittleFs()          override;
    IEspHttpServer   &getHttpServer()         override;
    IEspNvs          &getNvs()                override;
    IEspLogger       &getLogger()             override;
    IFreeRtosFactory &getFreeRtosFactory()    override;

private:
    EspLittleFs      littleFs_;
    EspHttpServer    httpServer_;
    EspNvs           nvs_;
    EspLogger        logger_;
    FreeRtosFactory  freeRtosFactory_;
};
