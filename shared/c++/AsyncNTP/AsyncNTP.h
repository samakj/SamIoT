#ifndef Async_NTP_h
#define Async_NTP_h

#include <Arduino.h>
#include <string>
#include <time.h>

#include <Log.h>
#include <AsyncWifi.h>
#include <TimeUtils.h>


class AsyncNTPClass {
    public:
        typedef std::function<void()> ConnectCallback;

        AsyncNTPClass (const AsyncNTPClass&) = delete;
        static AsyncNTPClass& getInstance();

        std::string server = "uk.pool.ntp.org";
        int timezone = 0;
        int dst = 0;
        int maxWait = 60;

        std::vector<ConnectCallback> connectCallbacks;

    private:
        bool usingWifiCallback = false;
        unsigned long lastReconnect = 0;

    public:
        bool isConnected();
        void addConnectCallback(ConnectCallback callback);
        void addWifiConnectCallback();
        void connect();
        void loop();
        void setServer(std::string server);
        void setTimezone(int timezone);
        void setDST(int dst);
        void setMaxWait(int maxWait);
    
    private:
        AsyncNTPClass() {};
        static AsyncNTPClass _instance;

        void wifiConnectCallback(std::string ssid);
};

static AsyncNTPClass& AsyncNTP = AsyncNTPClass::getInstance();

#endif