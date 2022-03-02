#include "AsyncOTA.h"

AsyncOTAClass AsyncOTAClass::_instance;

AsyncOTAClass &AsyncOTAClass::getInstance()
{
    return _instance;
};

void AsyncOTAClass::setup(std::string hostname, std::string password)
{
    ArduinoOTA.setHostname(hostname.c_str());
    ArduinoOTA.setPassword(password.c_str());

    ArduinoOTA.onStart([]()
                       {
                           std::string type;
                           if (ArduinoOTA.getCommand() == U_FLASH)
                               type = "sketch";
                           else
                               type = "filesystem";
                           Sam::Log.info("Start updating " + type);
                       });

    ArduinoOTA.onEnd([]()
                     { Sam::Log.info("\nOTA upload complete, rebooting."); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Sam::Log.infof("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError([](ota_error_t error)
                       {
                           Sam::Log.errorf("Error[%u]: ", error);
                           if (error == OTA_AUTH_ERROR)
                               Sam::Log.error("Auth Failed");
                           else if (error == OTA_BEGIN_ERROR)
                               Sam::Log.error("Begin Failed");
                           else if (error == OTA_CONNECT_ERROR)
                               Sam::Log.error("Connect Failed");
                           else if (error == OTA_RECEIVE_ERROR)
                               Sam::Log.error("Receive Failed");
                           else if (error == OTA_END_ERROR)
                               Sam::Log.error("End Failed");
                       });

    ArduinoOTA.begin();
    Sam::Log.info("OTA initialised");
    TelnetStream.begin();
    Sam::Log.info("Telnet initialised");
};

void AsyncOTAClass::loop()
{
    ArduinoOTA.handle();
};