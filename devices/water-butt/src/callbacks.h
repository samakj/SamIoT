#ifndef callbacks_h
#define callbacks_h

#include "config.h"
#include "defs.h"
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

void onTemperatureChange(float temperature)
{
    SamIoT::State::Measurements::set(temperature, "temperature", TEMPERATURE_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            temperature,
            "temperature",
            TEMPERATURE_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Temperature changed to %.1fc\n", temperature);
    Ultrasonic.setTemperature(temperature);
};

void onHumidityChange(float humidity)
{
    SamIoT::State::Measurements::set(humidity, "humidity", HUMIDITY_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            humidity,
            "humidity",
            HUMIDITY_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Humidity changed to %.1f%%\n", humidity);
};

void onDepthChange(float depth)
{
    SamIoT::State::Measurements::set(depth, "depth", DEPTH_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            depth,
            "depth",
            DEPTH_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Depth changed to %.3fm\n", depth);
};

#endif