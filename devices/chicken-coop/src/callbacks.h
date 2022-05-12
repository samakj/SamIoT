#ifndef callbacks_h
#define callbacks_h

#include "config.h"
#include "defs.h"
#include "display.h"
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
    Display::Views::Main::updateWifi(SamIoT::Wifi::ssid, SamIoT::Wifi::strength);
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
    Display::Views::Main::updateWifi(SamIoT::Wifi::ssid, SamIoT::Wifi::strength);
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
    SamIoT::Logger::infof("Wifi strength chaned to %.0f%%\n", strength * 100);
    Display::Views::Main::updateWifi(SamIoT::Wifi::ssid, SamIoT::Wifi::strength * 100);
};

void onNTPConnect(){};

void onCoopTemperatureChange(float temperature)
{
    SamIoT::State::Measurements::set(temperature, "temperature", COOP_TEMPERATURE_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            temperature,
            "temperature",
            COOP_TEMPERATURE_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Coop Temperature changed to %.1fc\n", temperature);
    Display::Views::Main::updateCoopTemperature(temperature);
};

void onCoopHumidityChange(float humidity)
{
    SamIoT::State::Measurements::set(humidity, "humidity", COOP_HUMIDITY_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            humidity,
            "humidity",
            COOP_HUMIDITY_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Coop Humidity changed to %.1f%%\n", humidity);
    Display::Views::Main::updateCoopHumidity(humidity);
};

void onShadeTemperatureChange(float temperature)
{
    SamIoT::State::Measurements::set(temperature, "temperature", SHADE_TEMPERATURE_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            temperature,
            "temperature",
            SHADE_TEMPERATURE_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Shade Temperature changed to %.1fc\n", temperature);
    Display::Views::Main::updateShadeTemperature(temperature);
};

void onShadeHumidityChange(float humidity)
{
    SamIoT::State::Measurements::set(humidity, "humidity", SHADE_HUMIDITY_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            humidity,
            "humidity",
            SHADE_HUMIDITY_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Shade Humidity changed to %.1f%%\n", humidity);
    Display::Views::Main::updateShadeHumidity(humidity);
};

void onRunTemperatureChange(float temperature)
{
    SamIoT::State::Measurements::set(temperature, "temperature", RUN_TEMPERATURE_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            temperature,
            "temperature",
            RUN_TEMPERATURE_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Run Temperature changed to %.1fc\n", temperature);
    Display::Views::Main::updateRunTemperature(temperature);
};

void onRunHumidityChange(float humidity)
{
    SamIoT::State::Measurements::set(humidity, "humidity", RUN_HUMIDITY_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            humidity,
            "humidity",
            RUN_HUMIDITY_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("Run Humidity changed to %.1f%%\n", humidity);
    Display::Views::Main::updateRunHumidity(humidity);
};

#endif