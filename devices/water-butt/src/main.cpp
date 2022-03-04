#include <Arduino.h>

#include "callbacks.h"
#include "config.h"
#include "defs.h"
#include "tags.h"
#include <Logger.h>
#include <Mesh.h>
#include <OTA.h>
#include <State.h>
#include <Time.h>

void setup()
{
    Serial.begin(115200);

    SamIoT::Logger::info("---------------- SETTING UP ----------------");
    SamIoT::Mesh::setWifiCredentials({&Patty, &Selma, &TheVale});
    SamIoT::Mesh::setMeshCredentials(&Marge);
    SamIoT::Mesh::setHostname(HOSTNAME);
    SamIoT::Mesh::setIpAddress({192, 168, 1, IP_LOCATION});

    SamIoT::Mesh::addConnectCallback(onWifiConnect);
    SamIoT::Mesh::addSsidCallback(onWifiSsidChange);
    SamIoT::Mesh::addStrengthCallback(onWifiStrengthChange);

    DHT.setTemperatureCallback(onTemperatureChange);
    DHT.setHumidityCallback(onHumidityChange);
    Ultrasonic.setDistanceCallback(onDepthChange);

    SamIoT::Mesh::setup();

    DHT.setup();
    Ultrasonic.setup();

    SamIoT::Logger::info("--------------- SETTING DONE ---------------");
}

void loop()
{
    SamIoT::Time::Timer::start("Loop");

    SamIoT::Time::Timer::start("Mesh");
    SamIoT::Mesh::loop();
    SamIoT::Time::Timer::end("Mesh");

    SamIoT::Time::Timer::start("OTA");
    SamIoT::OTA::loop();
    SamIoT::Time::Timer::end("OTA");

    SamIoT::Time::Timer::start("DHT");
    DHT.loop();
    SamIoT::Time::Timer::end("DHT");

    SamIoT::Time::Timer::start("Ultsonic");
    Ultrasonic.loop();
    SamIoT::Time::Timer::end("Ultsonic");

    SamIoT::Time::Timer::end("Loop");
    SamIoT::Time::Timer::loop();
}