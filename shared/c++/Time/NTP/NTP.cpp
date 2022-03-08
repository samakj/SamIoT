#include "NTP.h"

bool SamIoT::Time::NTP::isConnected()
{
    return time(nullptr) > 1616000000;
};

void SamIoT::Time::NTP::addConnectCallback(SamIoT::Time::NTP::ConnectCallback callback)
{
    SamIoT::Time::NTP::connectCallbacks.push_back(callback);
}

void SamIoT::Time::NTP::connect()
{
    SamIoT::Logger::infof("NTP not connected, connecting to %s..\n", server.c_str());
    configTime(
        SamIoT::Time::NTP::timezone * 3600,
        SamIoT::Time::NTP::dst * 3600,
        SamIoT::Time::NTP::server.c_str());
};

void SamIoT::Time::NTP::loop()
{
    if (!SamIoT::Time::NTP::isConnected() && !connecting)
        connect();

    if (SamIoT::Time::NTP::isConnected() && connecting)
    {
        SamIoT::Logger::infof(
            "Synced with '%s'. Time: %s.\n",
            SamIoT::Time::NTP::server.c_str(),
            SamIoT::Time::getIsoTimestamp().c_str());

        for (
            SamIoT::Time::NTP::ConnectCallback callback :
            SamIoT::Time::NTP::connectCallbacks)
            callback();

        connecting = false;
    }
};

void SamIoT::Time::NTP::setServer(std::string _server)
{
    SamIoT::Time::NTP::server = _server;
};

void SamIoT::Time::NTP::setTimezone(uint16_t _timezone)
{
    SamIoT::Time::NTP::timezone = _timezone;
};

void SamIoT::Time::NTP::setDST(uint16_t _dst)
{
    SamIoT::Time::NTP::dst = _dst;
};

void SamIoT::Time::NTP::setMaxWait(uint16_t _maxWait)
{
    SamIoT::Time::NTP::maxWait = _maxWait;
};