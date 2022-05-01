#ifndef _SamIoT_Time_NTP_h
#define _SamIoT_Time_NTP_h

#include <Arduino.h>
#include <string>
#include <time.h>

#include <Logger.h>

namespace SamIoT::Time
{
    unsigned long millisDiff(unsigned long start, unsigned long end);
    unsigned long millisSince(unsigned long start);
    std::string getIsoTimestamp();
    std::string formatTime(const char *format);
}

namespace SamIoT::Time::NTP
{
    typedef std::function<void()> ConnectCallback;

    extern std::string server;
    extern uint16_t timezone;
    extern uint16_t dst;
    extern uint16_t maxWait;

    extern std::vector<ConnectCallback> connectCallbacks;
    extern bool connecting;

    void connect();
    void loop();

    bool isConnected();

    void addConnectCallback(ConnectCallback callback);

    void setServer(std::string server);
    void setTimezone(uint16_t timezone);
    void setDST(uint16_t dst);
    void setMaxWait(uint16_t maxWait);
};

#endif