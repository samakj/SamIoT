#ifndef Async_Mesh_h
#define Async_Mesh_h

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

#include <Log.h>
#include <TimeUtils.h>

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

class AsyncMeshClass
{
public:
    typedef std::function<void(std::string ssid)> ConnectCallback;
    typedef std::function<void(std::string ssid)> SsidCallback;
    typedef std::function<void(float strength)> StrengthCallback;

    float STRENGTH_NULL_VALUE = -1;
    std::string HOSTNAME_NULL_VALUE = "";
    std::string SSID_NULL_VALUE = "";

    painlessMesh *meshClient = nullptr;

    std::vector<WifiCredentials *> wifiCredentials = {};
    MeshCredentials *meshCredentials = {};

    IPAddress ip = {0, 0, 0, 0};
    IPAddress gateway = {192, 168, 1, 1};
    IPAddress subnet = {255, 255, 0, 0};
    IPAddress dns1 = {8, 8, 8, 8};
    IPAddress dns2 = {4, 4, 4, 4};
    byte mac[6] = {0, 0, 0, 0, 0, 0};
    std::string hostname = "";

    float strength = -1;
    std::string ssid = "";

    uint16_t strengthUpdatePeriod = 10000;

    std::vector<ConnectCallback> connectCallbacks = {};
    std::vector<SsidCallback> ssidCallbacks = {};
    std::vector<StrengthCallback> strengthCallbacks = {};

private:
    unsigned long lastStrengthCheck = 0;
    std::unordered_map<uint32_t, std::string> nodeInfo = {};

public:
    AsyncMeshClass(const AsyncMeshClass &) = delete;
    static AsyncMeshClass &getInstance();

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
    WifiCredentials *getStrongestWifiNetwork();
    float getConnectionStrength();
    std::string getConnectionSsid();

    void meshResponse(AsyncWebServerRequest *request);

    void _messageReceivedCallback(const uint32_t &from, const String &message);
    void _newConnectionCallback(uint32_t nodeId);
    void _changedConnectionCallback();
    void _nodeTimeAdjustedCallback(int32_t offset);
    void _nodeDelayReceived(uint32_t nodeId, int32_t delay);

private:
    AsyncMeshClass(){};
    static AsyncMeshClass _instance;

    std::string getNodeInfo();
};

static AsyncMeshClass &AsyncMesh = AsyncMeshClass::getInstance();

#endif
