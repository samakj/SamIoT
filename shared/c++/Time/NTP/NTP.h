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

    static std::string server = "uk.pool.ntp.org";
    static uint16_t timezone = 0;
    static uint16_t dst = 0;
    static uint16_t maxWait = 20000;

    static std::vector<ConnectCallback> connectCallbacks = {};

    static bool usingWifiCallback = false;
    static unsigned long lastReconnect = 0;

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