#ifndef callbacks_h
#define callbacks_h

#include "config.h"
#include "tags.h"
#include <OTA.h>
#include <State.h>
#include <Time.h>
#include <WebServer.h>

void onWifiConnect(std::string ssid)
{
    SamIoT::State::Measurements::set(ssid, "ssid", WIFI_TAGS);
    SamIoT::OTA::setup(HOSTNAME, OTA_PASSWORD);
    SamIoT::Time::NTP::connect();
    SamIoT::Server::setup(true);
};

void onWifiSsidChange(std::string ssid)
{
    SamIoT::State::Measurements::set(ssid, "ssid", WIFI_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseString(
            ssid,
            "ssid",
            WIFI_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
};

void onWifiStrengthChange(float strength)
{
    SamIoT::State::Measurements::set(strength, "strength", WIFI_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            strength,
            "strength",
            WIFI_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Wifi strength chaned to %.0f%%\n", strength);
};

void onNTPConnect(){};

#endif