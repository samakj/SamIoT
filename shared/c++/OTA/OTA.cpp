#include "OTA.h"

void SamIoT::OTA::setup(std::string hostname, std::string password)
{
    ArduinoOTA.setHostname(hostname.c_str());
    ArduinoOTA.setPassword(password.c_str());

    ArduinoOTA.onStart(
        []()
        {
            std::string type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else
                type = "filesystem";
            SamIoT::Logger::info("Start updating " + type);
        });

    ArduinoOTA.onEnd(
        []()
        { SamIoT::Logger::info("\nOTA upload complete, rebooting."); });

    ArduinoOTA.onProgress(
        [](unsigned int progress, unsigned int total)
        { SamIoT::Logger::infof("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError(
        [](ota_error_t error)
        {
            SamIoT::Logger::errorf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                SamIoT::Logger::error("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                SamIoT::Logger::error("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                SamIoT::Logger::error("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                SamIoT::Logger::error("Receive Failed");
            else if (error == OTA_END_ERROR)
                SamIoT::Logger::error("End Failed");
        });

    ArduinoOTA.begin();
    SamIoT::Logger::info("OTA initialised");
    TelnetStream.begin();
    SamIoT::Logger::info("Telnet initialised");
};

void SamIoT::OTA::loop()
{
    ArduinoOTA.handle();
};