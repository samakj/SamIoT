#ifndef _SamIoT_Mesh_h
#define _SamIoT_Mesh_h

#include <Arduino.h>
#include <functional>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#else
#include <AsyncTCP.h>
#include <WiFi.h>
#endif

#include <ESPAsyncWebServer.h>
#include <painlessMesh.h>

#include <Logger.h>
#include <Time.h>

namespace SamIoT::Mesh
{
    struct WifiCredentials
    {
        std::string ssid;
        std::string password;
    };

    struct MeshCredentials
    {
        std::string ssid;
        std::string password;
        uint16_t port = 5555;
    };

    typedef std::function<void(std::string ssid)> ConnectCallback;
    typedef std::function<void(std::string ssid)> SsidCallback;
    typedef std::function<void(float strength)> StrengthCallback;

    static float STRENGTH_NULL_VALUE = -1;
    static std::string HOSTNAME_NULL_VALUE = "";
    static std::string SSID_NULL_VALUE = "";

    static painlessMesh *meshClient = nullptr;

    static std::vector<WifiCredentials *> wifiCredentials = {};
    static MeshCredentials *meshCredentials = {};

    static IPAddress ip = {0, 0, 0, 0};
    static IPAddress gateway = {192, 168, 1, 1};
    static IPAddress subnet = {255, 255, 0, 0};
    static IPAddress dns1 = {8, 8, 8, 8};
    static IPAddress dns2 = {4, 4, 4, 4};
    static byte mac[6] = {0, 0, 0, 0, 0, 0};
    static std::string hostname = "";

    static float strength = -1;
    static std::string ssid = "";

    static uint16_t strengthUpdatePeriod = 10000;

    static std::vector<ConnectCallback> connectCallbacks = {};
    static std::vector<SsidCallback> ssidCallbacks = {};
    static std::vector<StrengthCallback> strengthCallbacks = {};

    static unsigned long lastStrengthCheck = 0;
    static std::unordered_map<uint32_t, std::string> nodeInfo = {};

    void setup();
    void loop();

    void addConnectCallback(ConnectCallback callback);
    void addSsidCallback(SsidCallback callback);
    void addStrengthCallback(StrengthCallback callback);

    void setWifiCredentials(std::vector<WifiCredentials *> wifiCredentials = {});
    void setMeshCredentials(MeshCredentials *meshCredentials = {});
    void setIpAddress(
        IPAddress ip = {0, 0, 0, 0},
        IPAddress gateway = {192, 168, 1, 1},
        IPAddress subnet = {255, 255, 0, 0},
        IPAddress dns1 = {8, 8, 8, 8},
        IPAddress dns2 = {4, 4, 4, 4});
    void setHostname(std::string hostname = "");

    std::string getIPAddressString();
    std::string getMACAddressString();
    void connectToNetwork();
    float getConnectionStrength();
    std::string getConnectionSsid();

    void meshResponse(AsyncWebServerRequest *request);

    void _messageReceivedCallback(const uint32_t &from, const String &message);
    void _newConnectionCallback(uint32_t nodeId);
    void _changedConnectionCallback();
    void _nodeTimeAdjustedCallback(int32_t offset);
    void _nodeDelayReceived(uint32_t nodeId, int32_t delay);

    std::string getNodeInfo();
};

#endif
