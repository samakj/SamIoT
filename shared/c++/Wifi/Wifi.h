#ifndef _SamIoT_Wifi_h
#define _SamIoT_Wifi_h

#include <Arduino.h>
#include <string>
#include <vector>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <Logger.h>
#include <Time.h>

namespace SamIoT::Wifi
{
    typedef std::function<void(std::string ssid)> ConnectCallback;
    typedef std::function<void(std::string ssid)> SsidCallback;
    typedef std::function<void(float strength)> StrengthCallback;

    class WifiCredentials
    {
    public:
        std::string ssid;
        std::string password;

    private:
    public:
        WifiCredentials(std::string _ssid, std::string _password);
    };

    static std::string SSID_NULL_VALUE = "";
    static float STRENGTH_NULL_VALUE = -1.0f;
    static uint8_t IP_LOCATION_NULL_VALUE = 0;

    static std::string ssid = SSID_NULL_VALUE;
    static float strength = STRENGTH_NULL_VALUE;
    static uint16_t strengthUpdatePeriod = 10000;
    static std::vector<ConnectCallback> connectCallbacks = {};
    static std::vector<SsidCallback> ssidCallbacks = {};
    static std::vector<StrengthCallback> strengthCallbacks = {};

    static unsigned long lastStrengthUpdate = 0;

    boolean isConnected();
    std::string MACAddressByteArrayToString(byte MACAddressByteArray[6]);
    std::string IPAddressToString(IPAddress ip);

    void addConnectCallback(ConnectCallback callback);
    void addSsidCallback(SsidCallback callback);
    void addStrengthCallback(StrengthCallback callback);

    std::string getLocalIPAddressString();
    std::string getMACAddressString();
    float getConnectedNetworkStrength();
    std::string getConnectedNetworkSsid();
    float getNetworkStrength(std::string ssid);
    WifiCredentials *getStrongestNetwork(
        std::vector<WifiCredentials *> credentials);

    void connect(
        WifiCredentials *credentials,
        std::string hostname,
        uint8_t ipLocation = 1,
        uint16_t maxWait = 60000);
    void connect(
        std::vector<WifiCredentials *> credentials,
        std::string hostname,
        uint8_t ipLocation = 1,
        uint16_t maxWait = 60000);

    void _connect(
        WifiCredentials *credentials,
        std::string hostname = nullptr,
        uint8_t ipLocation = 1,
        uint16_t maxWait = 60000);
};

#endif