#include "NTP.h"

namespace SamIoT::Time::NTP
{
    std::string server = "uk.pool.ntp.org";
    uint16_t timezone = 0;
    uint16_t dst = 0;
    uint16_t maxWait = 20000;

    std::vector<SamIoT::Time::NTP::ConnectCallback> connectCallbacks = {};
    bool connecting = false;

    bool isConnected()
    {
        return time(nullptr) > 1616000000;
    };

    void addConnectCallback(SamIoT::Time::NTP::ConnectCallback callback)
    {
        SamIoT::Time::NTP::connectCallbacks.push_back(callback);
    }

    void connect()
    {
        SamIoT::Logger::infof("NTP not connected, connecting to %s..\n", server.c_str());
        configTime(
            SamIoT::Time::NTP::timezone * 3600,
            SamIoT::Time::NTP::dst * 3600,
            SamIoT::Time::NTP::server.c_str());
    };

    void loop()
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

    void setServer(std::string _server)
    {
        SamIoT::Time::NTP::server = _server;
    };

    void setTimezone(uint16_t _timezone)
    {
        SamIoT::Time::NTP::timezone = _timezone;
    };

    void setDST(uint16_t _dst)
    {
        SamIoT::Time::NTP::dst = _dst;
    };

    void setMaxWait(uint16_t _maxWait)
    {
        SamIoT::Time::NTP::maxWait = _maxWait;
    };
}