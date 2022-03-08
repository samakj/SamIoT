#include <Arduino.h>

#include "callbacks.h"
#include "config.h"
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

    SamIoT::Mesh::setup(true);

    SamIoT::Logger::info("-------------- SETTING UP DONE -------------");
}

unsigned long lastReport = 0;

void loop()
{
    SamIoT::Time::Timer::start("Loop");

    if (
        SamIoT::Mesh::meshClient != nullptr &&
        SamIoT::Time::millisSince(lastReport) > 10000)
    {
        uint8_t count = SamIoT::Mesh::meshClient->getNodeList().size();
        SamIoT::Logger::infof("%u nodes on mesh.\n", count);
        lastReport = millis();
    }

    SamIoT::Time::Timer::start("Mesh");
    SamIoT::Mesh::loop();
    SamIoT::Time::Timer::end("Mesh");

    SamIoT::Time::Timer::start("OTA");
    SamIoT::OTA::loop();
    SamIoT::Time::Timer::end("OTA");

    SamIoT::Time::Timer::end("Loop");
    SamIoT::Time::Timer::loop();
}