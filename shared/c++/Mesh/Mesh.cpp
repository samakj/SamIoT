#include "Mesh.h"

void SamIoT::Mesh::setup(bool isBridge)
{
    SamIoT::Mesh::WifiCredentials* wifi = isBridge ? SamIoT::Mesh::getStrongestWifiNetwork() : nullptr;

    SamIoT::Mesh::meshClient = new painlessMesh();

    SamIoT::Mesh::meshClient->setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );

    SamIoT::Mesh::meshClient->init(
        SamIoT::Mesh::meshCredentials->ssid.c_str(),
        SamIoT::Mesh::meshCredentials->password.c_str(),
        SamIoT::Mesh::meshCredentials->port,
        WIFI_AP_STA);

    SamIoT::Mesh::meshClient->onReceive(SamIoT::Mesh::_messageReceivedCallback);
    SamIoT::Mesh::meshClient->onNewConnection(SamIoT::Mesh::_newConnectionCallback);
    SamIoT::Mesh::meshClient->onChangedConnections(SamIoT::Mesh::_changedConnectionCallback);
    SamIoT::Mesh::meshClient->onNodeTimeAdjusted(SamIoT::Mesh::_nodeTimeAdjustedCallback);
    SamIoT::Mesh::meshClient->onNodeDelayReceived(SamIoT::Mesh::_nodeDelayReceived);

    if (SamIoT::Mesh::hostname != SamIoT::Mesh::HOSTNAME_NULL_VALUE)
    {
        SamIoT::Mesh::meshClient->setHostname(SamIoT::Mesh::hostname.c_str());
        SamIoT::Mesh::meshClient->initOTAReceive(SamIoT::Mesh::hostname.c_str());
    }

    if (isBridge) {
        SamIoT::Mesh::meshClient->stationManual(
            wifi->ssid.c_str(),
            wifi->password.c_str());
        SamIoT::Mesh::meshClient->setRoot(true);
        SamIoT::Mesh::meshClient->setContainsRoot(true);
    }

    WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
    {
        SamIoT::Logger::info("Connected to mesh:");
        SamIoT::Logger::infof("    Node ID:  %X\n", SamIoT::Mesh::meshClient->getNodeId());
        SamIoT::Logger::infof("    IP:       %s\n", SamIoT::Mesh::getIPAddressString().c_str());
        SamIoT::Logger::infof("    MAC:      %s\n", SamIoT::Mesh::getMACAddressString().c_str());
        SamIoT::Logger::infof("    Hostname: %s\n", WiFi.getHostname());
        for (SamIoT::Mesh::SsidCallback callback : SamIoT::Mesh::connectCallbacks)
            callback(SamIoT::Mesh::ssid);
    });
};

void SamIoT::Mesh::loop()
{
    if (SamIoT::Mesh::meshClient == nullptr)
        SamIoT::Mesh::setup();
    if (SamIoT::Mesh::meshClient != nullptr)
        SamIoT::Mesh::meshClient->update();
};

void SamIoT::Mesh::addConnectCallback(ConnectCallback callback)
{
    SamIoT::Mesh::connectCallbacks.push_back(callback);
};

void SamIoT::Mesh::addSsidCallback(SsidCallback callback)
{
    SamIoT::Mesh::ssidCallbacks.push_back(callback);
};

void SamIoT::Mesh::addStrengthCallback(StrengthCallback callback)
{
    SamIoT::Mesh::strengthCallbacks.push_back(callback);
};

void SamIoT::Mesh::setWifiCredentials(std::vector<WifiCredentials *> _wifiCredentials)
{
    SamIoT::Mesh::wifiCredentials = _wifiCredentials;
};

void SamIoT::Mesh::setMeshCredentials(MeshCredentials *_meshCredentials)
{
    SamIoT::Mesh::meshCredentials = _meshCredentials;
};

void SamIoT::Mesh::setIpAddress(
    IPAddress _ip,
    IPAddress _gateway,
    IPAddress _subnet,
    IPAddress _dns1,
    IPAddress _dns2)
{
    SamIoT::Mesh::ip = _ip;
    SamIoT::Mesh::gateway = _gateway;
    SamIoT::Mesh::subnet = _subnet;
    SamIoT::Mesh::dns1 = _dns1;
    SamIoT::Mesh::dns2 = _dns2;

    if (!WiFi.config(
            SamIoT::Mesh::ip,
            SamIoT::Mesh::gateway,
            SamIoT::Mesh::subnet,
            SamIoT::Mesh::dns1,
            SamIoT::Mesh::dns2))
    {
        SamIoT::Logger::error("Failed to set IP information.");
    }
};
void SamIoT::Mesh::setHostname(std::string _hostname)
{
    SamIoT::Mesh::hostname = _hostname;
    SamIoT::Mesh::meshClient->setHostname(SamIoT::Mesh::hostname.c_str());
};

std::string SamIoT::Mesh::getMACAddressString()
{
    byte mac[6];
    WiFi.macAddress(mac);
    char buffer[32];
    sprintf(
        buffer,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],
        mac[1],
        mac[2],
        mac[3],
        mac[4],
        mac[5]);
    return (std::string)buffer;
};

std::string SamIoT::Mesh::getIPAddressString()
{
    if (SamIoT::Mesh::meshClient == nullptr)
        return "";

    IPAddress ip = SamIoT::Mesh::meshClient->getStationIP();
    char buffer[32];
    sprintf(
        buffer,
        "%d.%d.%d.%d",
        ip[0],
        ip[1],
        ip[2],
        ip[3]);
    return (std::string)buffer;
};

SamIoT::Mesh::WifiCredentials* SamIoT::Mesh::getStrongestWifiNetwork()
{
    if (!SamIoT::Mesh::wifiCredentials.size())
    {
        SamIoT::Logger::error("No wifi credentials to connect to.");
        return nullptr;
    }

    SamIoT::Logger::infof(
        "Finding strongest of %u wifi networks...\n",
        (uint8_t)SamIoT::Mesh::wifiCredentials.size());

    SamIoT::Logger::info("Scanning local networks...");

    uint8_t networkCount = WiFi.scanNetworks();

    SamIoT::Logger::infof("%u networks found in range.\n", networkCount);

    bool meshFound = false;
    SamIoT::Mesh::WifiCredentials* strongest = nullptr;
    float strengthOfStrongest = 0;

    for (uint8_t i = 0; i < networkCount; i++)
    {
        float _strength = (100 + WiFi.RSSI(i)) * 2;
        std::string _ssid = WiFi.SSID(i).c_str();
        bool match = false;

        if (SamIoT::Mesh::meshCredentials->ssid == _ssid)
        {
            match = true;
            SamIoT::Logger::infof(
                "[MESH] '%s' network found with strength %.1f%%.\n",
                _ssid.c_str(),
                _strength);
            meshFound = true;
        }

        if (!match)
        {
            for (WifiCredentials *_credential : wifiCredentials)
            {
                if (_credential->ssid == WiFi.SSID(i).c_str())
                {
                    match = true;
                    SamIoT::Logger::infof(
                        "[WIFI] '%s' network found with strength %.1f%%.\n",
                        WiFi.SSID(i),
                        _strength);
                    if (_strength > strengthOfStrongest)
                    {
                        strongest = _credential;
                        strengthOfStrongest = _strength;
                    }
                }
            }
        }

        if (!match)
            SamIoT::Logger::infof(
                "       '%s' network found with strength %.1f%%.\n",
                WiFi.SSID(i),
                _strength);
    }

    if (!meshFound)
        SamIoT::Logger::warn("Mesh not found in wifi network scan.");
    if (strongest==nullptr)
        SamIoT::Logger::error("No matching credentials in wifi network scan.");
    else {
        SamIoT::Logger::infof(
            "'%s' network strongest at %.1f%%\n",
            strongest->ssid.c_str(), 
            strengthOfStrongest);
        ssid = strongest->ssid;
        strength = strengthOfStrongest;
    }

    return strongest;
};

float SamIoT::Mesh::getConnectionStrength()
{
    // if (SamIoT::Time::millisSince(lastStrengthCheck) > SamIoT::Mesh::strengthUpdatePeriod)
    // {
    //     float _strength = WiFi.RSSI() * 2;
    //     if (_strength != strength)
    //     {
    //         SamIoT::Mesh::strength = _strength;
    //         for (StrengthCallback callback : SamIoT::Mesh::strengthCallbacks)
    //             callback(SamIoT::Mesh::strength);
    //     }
    //     SamIoT::Mesh::lastStrengthCheck = millis();
    // }

    return SamIoT::Mesh::strength;
};

std::string SamIoT::Mesh::getConnectionSsid()
{
    // std::string _ssid = (std::string)(WiFi.SSID().c_str());

    // if (_ssid.size() && _ssid != SamIoT::Mesh::ssid)
    // {
    //     SamIoT::Mesh::ssid = _ssid;
    //     for (SsidCallback callback : SamIoT::Mesh::ssidCallbacks)
    //         callback(SamIoT::Mesh::ssid);
    // }

    return SamIoT::Mesh::ssid;
};

std::string SamIoT::Mesh::getNodeInfo()
{
    std::string response = "{";

    response += "\"ip\":\"";
    response += SamIoT::Mesh::getIPAddressString();
    response += "\",";

    response += "\"nodeId\":";
    char buff[16];
    sprintf(buff, "%u", SamIoT::Mesh::meshClient->getNodeId());
    response += buff;
    response += ",";

    response += "\"mac\":\"";
    response += SamIoT::Mesh::getMACAddressString();
    response += "\",";

    response += "\"hostname\":";
    response += "\"";
    response += WiFi.getHostname();
    response += "\",";

    response += "\"strength\":";
    if (SamIoT::Mesh::strength != SamIoT::Mesh::STRENGTH_NULL_VALUE)
    {
        char buff[64];
        sprintf(buff, "%f", SamIoT::Mesh::strength);
        response += buff;
    }
    else
        response += "null";
    response += ",";

    response += "\"ssid\":";
    if (SamIoT::Mesh::ssid != SamIoT::Mesh::SSID_NULL_VALUE)
    {
        response += "\"";
        response += SamIoT::Mesh::ssid;
        response += "\"";
    }
    else
        response += "null";
    response += ",";

    response += "\"isRoot\":";
    response += SamIoT::Mesh::meshClient->isRoot() ? "true" : "false";
    response += "";

    response += "}";

    return response;
}

void SamIoT::Mesh::meshResponse(AsyncWebServerRequest *request)
{
    unsigned long requestStart = millis();
    SamIoT::Mesh::meshClient->sendBroadcast("INFO?");
    std::list<uint32_t> nodeList = SamIoT::Mesh::meshClient->getNodeList();
    String connectionJson = SamIoT::Mesh::meshClient->subConnectionJson();
    SamIoT::Mesh::nodeInfo[SamIoT::Mesh::meshClient->getNodeId()] = getNodeInfo();

    while (
        SamIoT::Mesh::nodeInfo.size() < nodeList.size() &&
        SamIoT::Time::millisSince(requestStart) < 1000)
    {
        yield();
        delay(10);
    }

    std::string json = "{\"connections\":";
    json += connectionJson.c_str();
    json += ",\"info\":{";

    for (auto &it : SamIoT::Mesh::nodeInfo)
    {
        json += "\"";
        char buff[16];
        sprintf(buff, "%u", it.first);
        json += buff;
        json += "\":";
        json += it.second;
        json += ",";
    }

    json.pop_back();
    json += "}}";
    request->send(200, "application/json", json.c_str());
    SamIoT::Mesh::nodeInfo = {};
    json = "";
};

void SamIoT::Mesh::_messageReceivedCallback(const uint32_t &from, const String &message)
{
    SamIoT::Logger::infof("Mesh message received from %u message = '%s'\n", from, message.c_str());
    if (message.startsWith("INFO?"))
    {
        std::string response = "INFO>";
        response += getNodeInfo();
        SamIoT::Mesh::meshClient->sendSingle(from, response.c_str());
        return;
    }
    if (message.startsWith("INFO?"))
    {
        std::string _message = message.c_str();
        _message.replace(0, 5, "");
        SamIoT::Mesh::nodeInfo[from] = _message;
        return;
    }
};

void SamIoT::Mesh::_newConnectionCallback(uint32_t nodeId)
{
    SamIoT::Logger::infof("New mesh connection, nodeId = %u\n", nodeId);
};

void SamIoT::Mesh::_changedConnectionCallback()
{
    SamIoT::Logger::infof("Mesh changed connections\n");
};

void SamIoT::Mesh::_nodeTimeAdjustedCallback(int32_t offset)
{
    SamIoT::Logger::infof("Adjusted mesh time %u. Offset = %d\n", meshClient->getNodeTime(), offset);
};

void SamIoT::Mesh::_nodeDelayReceived(uint32_t nodeId, int32_t delay)
{
    SamIoT::Logger::infof("Mesh delay from node %u = %d\n", nodeId, delay);
};