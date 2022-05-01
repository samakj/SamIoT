#include <Arduino.h>

#include "callbacks.h"
#include "config.h"
#include "defs.h"
#include "display.h"
#include "tags.h"
#include <Logger.h>
#include <OTA.h>
#include <State.h>
#include <Time.h>
#include <Wifi.h>

void setup()
{
    Serial.begin(115200);

    SamIoT::Logger::info("---------------- SETTING UP ----------------");
    Display::setup();
    Display::Navigation.goToPage(Display::Views::Boot::name);
    Display::Views::Boot::updateTask("Connecting to wifi");
    Display::Renderer->draw();

    SamIoT::Wifi::connect({&Patty, &Selma, &TheVale}, HOSTNAME, IP_LOCATION);

    SamIoT::Wifi::addConnectCallback(onWifiConnect);
    SamIoT::Wifi::addSsidCallback(onWifiSsidChange);
    SamIoT::Wifi::addStrengthCallback(onWifiStrengthChange);

    CoopDHT.setTemperatureCallback(onCoopTemperatureChange);
    CoopDHT.setHumidityCallback(onCoopHumidityChange);
    ShadeDHT.setTemperatureCallback(onShadeTemperatureChange);
    ShadeDHT.setHumidityCallback(onShadeHumidityChange);
    RunDHT.setTemperatureCallback(onRunTemperatureChange);
    RunDHT.setHumidityCallback(onRunHumidityChange);

    SamIoT::Wifi::connect({&Patty, &Selma, &TheVale}, HOSTNAME, IP_LOCATION);

    CoopDHT.setup();
    ShadeDHT.setup();
    RunDHT.setup();

    SamIoT::Logger::info("--------------- SETTING DONE ---------------");
    Display::Navigation.goToPage(Display::Views::Main::name);
    Display::Renderer->draw();
}

void loop()
{
    SamIoT::Time::Timer::start("Loop");

    SamIoT::Time::Timer::start("Wifi");
    SamIoT::Wifi::connect({&Patty, &Selma, &TheVale}, HOSTNAME, IP_LOCATION);
    SamIoT::Time::Timer::end("Wifi");

    SamIoT::Time::Timer::start("OTA");
    SamIoT::OTA::loop();
    SamIoT::Time::Timer::end("OTA");

    SamIoT::Time::Timer::start("CoopDHT");
    CoopDHT.loop();
    SamIoT::Time::Timer::end("CoopDHT");

    SamIoT::Time::Timer::start("ShadeDHT");
    ShadeDHT.loop();
    SamIoT::Time::Timer::end("ShadeDHT");

    SamIoT::Time::Timer::start("RunDHT");
    RunDHT.loop();
    SamIoT::Time::Timer::end("RunDHT");

    SamIoT::Time::Timer::start("Screen");
    Display::Views::Main::updateTime();
    Display::Renderer->draw();
    SamIoT::Time::Timer::end("Screen");

    SamIoT::Time::Timer::end("Loop");
    SamIoT::Time::Timer::loop();
}