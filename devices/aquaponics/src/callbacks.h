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

void onNorthBedTemperatureChange(float temperature)
{
    SamIoT::State::Measurements::set(temperature, "temperature", NORTH_BED_TEMPERATURE_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            temperature,
            "temperature",
            NORTH_BED_TEMPERATURE_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("North Bed Temperature changed to %.1fc\n", temperature);
    NorthBedUltrasonic.setTemperature(temperature);
};

void onNorthBedHumidityChange(float humidity)
{
    SamIoT::State::Measurements::set(humidity, "humidity", NORTH_BED_HUMIDITY_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            humidity,
            "humidity",
            NORTH_BED_HUMIDITY_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("North Bed Humidity changed to %.1f%%\n", humidity);
};

void onNorthBedDepthChange(float depth)
{
    SamIoT::State::Measurements::set(depth, "depth", NORTH_BED_DEPTH_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            depth,
            "depth",
            NORTH_BED_DEPTH_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("North Bed Depth changed to %.3fm\n", depth);
};

void onSouthBedTemperatureChange(float temperature)
{
    SamIoT::State::Measurements::set(temperature, "temperature", SOUTH_BED_TEMPERATURE_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            temperature,
            "temperature",
            SOUTH_BED_TEMPERATURE_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("South Bed Temperature changed to %.1fc\n", temperature);
    SouthBedUltrasonic.setTemperature(temperature);
};

void onSouthBedHumidityChange(float humidity)
{
    SamIoT::State::Measurements::set(humidity, "humidity", SOUTH_BED_HUMIDITY_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            humidity,
            "humidity",
            SOUTH_BED_HUMIDITY_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("South Bed Humidity changed to %.1f%%\n", humidity);
};

void onSouthBedDepthChange(float depth)
{
    SamIoT::State::Measurements::set(depth, "depth", SOUTH_BED_DEPTH_TAGS);
    SamIoT::Server::sendMessage(
        SamIoT::State::Measurements::serialiseFloat(
            depth,
            "depth",
            SOUTH_BED_DEPTH_TAGS,
            SamIoT::Server::getMessageMeta(
                SamIoT::Server::MessageType::MEASUREMENT)));
    SamIoT::Logger::infof("South Bed Depth changed to %.3fm\n", depth);
};

#endif