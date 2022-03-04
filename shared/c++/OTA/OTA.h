#ifndef _SamIoT_OTA_h
#define _SamIoT_OTA_h

#include <ArduinoOTA.h>
#include <TelnetStream.h>
#include <string>

#include <Logger.h>

namespace SamIoT::OTA
{
    void setup(std::string hostname, std::string password);
    void loop();
};

#endif