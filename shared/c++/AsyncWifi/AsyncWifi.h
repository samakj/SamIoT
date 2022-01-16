#ifndef Async_Wifi_h
#define Async_Wifi_h

#include <Arduino.h>
#include <string>
#include <vector>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <Log.h>
#include <TimeUtils.h>

static std::string SSID_NULL_VALUE = "";
static float STRENGTH_NULL_VALUE = -1.0f;

class WifiCredentials {
    public:
        std::string ssid;
        std::string password;
    
    private:

    public:
        WifiCredentials(std::string _ssid, std::string _password);
};

class AsyncWifiClass {
    public:
        typedef std::function<void(std::string ssid)> ConnectCallback;
        typedef std::function<void(std::string ssid)> SsidCallback;
        typedef std::function<void(float strength)> StrengthCallback;

        int IP_LOCATION_NULL_VALUE = -1;

        std::string ssid = SSID_NULL_VALUE;
        float strength = STRENGTH_NULL_VALUE;
        int strengthUpdatePeriod = 1;
        std::vector<ConnectCallback> connectCallbacks;
        std::vector<SsidCallback> ssidCallbacks;
        std::vector<StrengthCallback> strengthCallbacks;

    private:
        unsigned long _lastStrengthUpdate = 0;
    
    public:
        AsyncWifiClass (const AsyncWifiClass&) = delete;
        static AsyncWifiClass& getInstance();

        std::string MACAddressByteArrayToString(byte MACAddressByteArray[6]);
        std::string getMACAddressString();
        std::string IPAddressToString(IPAddress ip);
        std::string getLocalIPAddressString();
        boolean isConnected();
        void addConnectCallback(ConnectCallback callback);
        void addSsidCallback(SsidCallback callback);
        void addStrengthCallback(StrengthCallback callback);
        float getConnectedNetworkStrength();
        std::string getConnectedNetworkSsid();
        float getNetworkStrength(std::string ssid);
        WifiCredentials* getStrongestNetwork(
            std::vector<WifiCredentials*> credentials
        );
        void connect(
            WifiCredentials* credentials,
            std::string hostname,
            int ipLocation = -1,
            int maxWait = 60
        );
        void connect(
            std::vector<WifiCredentials*> credentials,
            std::string hostname,
            int ipLocation = -1,
            int maxWait = 60
        );

    private:
        AsyncWifiClass() {};
        static AsyncWifiClass _instance;

        void _connect(
            WifiCredentials* credentials,
            std::string hostname = nullptr,
            int ipLocation = -1,
            int maxWait = 60
        );
};

static AsyncWifiClass& AsyncWifi = AsyncWifiClass::getInstance();

#endif