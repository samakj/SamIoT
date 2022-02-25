#include "AsyncMesh.h"

AsyncMeshClass AsyncMeshClass::_instance;

void AsyncMeshClass::setup()
{
    WifiCredentials *_wifiCredentials = getStrongestWifiNetwork();

    if (_wifiCredentials == nullptr)
    {
        Log.error("No wifi credentials aborting mesh setup.")
    }

    meshClient = new painlessMesh();
    meshClient.init(
        meshCredentials.ssid.c_str(),
        meshCredentials.password.c_str(),
        meshCredentials.port,
        WIFI_AP_STA);

    meshClient.onReceive(&AsyncMesh._messageReceivedCallback);
    meshClient.onNewConnection(&AsyncMesh._newConnectionCallback);
    meshClient.onChangedConnections(&AsyncMesh._changedConnectionCallback);
    meshClient.onNodeTimeAdjusted(&AsyncMesh._nodeTimeAdjustedCallback);
    meshClient.onNodeDelayRecieved(&AsyncMesh._nodeDelayReceived);

    meshClient.stationManual(_wifiCredentials->ssid, wifiCredentials->password);

    if (hostname != HOSTNAME_NULL_VALUE)
        meshClient.setHostname(hostname);

    for (SsidCallback callback : connectCallbacks)
        callback(ssid);
    getConnectionSsid();
    getConnectionStrength();

    meshClient.setRoot(true);
    meshClient.setContainsRoot(true);
    meshClient.initOTAReceive("bridge");

    Log.infof("Connected to mesh, node id: %d", meshClient.getNodeId())
};

void AsyncMeshClass::loop()
{
    if (meshClient == nullptr)
        setup();
    if (meshClient != nullptr)
        meshClient.update();
};

void AsyncMeshClass::setWifiCredentials(std::vector<WifiCredentials> _wifiCredentials)
{
    wifiCredentials = _wifiCredentials;
};

void AsyncMeshClass::setMeshCredentials(MeshCredentials _meshCredentials)
{
    meshCredentials = _meshCredentials;
};

void AsyncMeshClass::setIpAddress(
    IPAddress _ip,
    IPAddress _gateway,
    IPAddress _subnet,
    IPAddress _dns1,
    IPAddress _dns2)
{
    ip = _ip;
    gateway = _gateway;
    subnet = _subnet;
    dns1 = _dns1;
    dns2 = _dns2;

    if (!WiFi.config(ip, gateway, subnet, dns1, dns2))
        Log.error("Failed to set IP information.");
};
void AsyncMeshClass::setHostname(std::string _hostname)
{
    hostname = _hostname;
    WiFi.setHostname(hostname.c_str());
};

AsyncMeshClass &AsyncMeshClass::getInstance()
{
    return _instance;
}

std::string AsyncMeshClass::getMACAddressString()
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

std::string AsyncMeshClass::getIPAddressString()
{
    if (meshClient == nullptr)
        return "";

    IPAddress ip = meshClient.getStationIP();
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

WifiCredentials *AsyncMeshClass::getStrongestWifiNetwork()
{
    if (!wifiCredentials.size())
    {
        Log.error("Can't find strongest wifi of empty credentials list.");
        return nullptr;
    }

    Log.infof("Finding strongest of %d networks...\n", credentials.size());
    Log.info("Scanning local networks...");

    int networkCount = WiFi.scanNetworks();

    Log.infof("%d networks found in range.\n", networkCount);

    WifiCredentials *strongest = nullptr;
    int strengthOfStrongest = 0;

    for (int i = 0; i < networkCount; i++)
    {
        float strength = (255 + WiFi.RSSI(i)) / 2.55;
        Log.infof(
            "        '%s' network found with strength %.1f%%.",
            WiFi.SSID(i),
            strength);
        for (WifiCredentials *_credential : credentials)
            if (_credential->ssid == WiFi.SSID(i).c_str())
            {
                Log.infof(
                    "\r[MATCH] '%s' network found with strength %.1f%%.",
                    WiFi.SSID(i),
                    strength);
                if (strength > strengthOfStrongest)
                {
                    strongest = _credential;
                    strengthOfStrongest = strength;
                }
            }
        Log.infof("\n")
    }
    if (!strongest)
    {
        Log.error("None of the provided credentials matched found local networks.");
    }
    else
    {
        Log.infof("'%s' found as the strongest.\n", strongest->ssid.c_str());
    }

    return strongest;
};

float AsyncMeshClass::getConnectionStrength()
{
    if (TimeUtils.millisSince(lastStrengthUpdate) > strengthUpdatePeriod)
    {
        float _strength = (255 + WiFi.RSSI()) / 255.0f;
        if (_strength != strength)
        {
            strength = _strength;
            for (StrengthCallback callback : strengthCallbacks)
                callback(strength);
        }
        lastStrengthUpdate = millis();
    }

    return strength;
};

std::string AsyncMeshClass::getConnectionSsid()
{
    std::string _ssid = (std::string)(WiFi.SSID().c_str());

    if (_ssid != ssid)
    {
        ssid = _ssid;
        for (SsidCallback callback : ssidCallbacks)
            callback(ssid);
    }

    return ssid;
};

std::string AsyncMeshClass::getNodeInfo()
{
    std::string response = "{";

    response += "\"ip\":\"";
    response += getIpAddressString();
    response += "\",";

    response += "\"mac\":\"";
    response += getMacAddressString();
    response += "\",";

    response += "\"hostname\":";
    if (hostname != HOSTNAME_NULL_VALUE)
    {
        response += "\"";
        response += hostname;
        response += "\"";
    }
    else
        response += "null";
    response += ",";

    response += "\"strength\":";
    if (hostname != STRENGTH_NULL_VALUE)
    {
        char buf[64];
        sprintf(buff, "%f", strength);
        response += buff;
    }
    else
        response += "null";
    response += ",";

    response += "\"ssid\":";
    if (hostname != SSID_NULL_VALUE)
    {
        response += "\"";
        response += ssid;
        response += "\"";
    }
    else
        response += "null";
    response += ",";

    response += "\"isRoot\":";
    response += meshClient.isRoot() ? "true" : "false";
    response += "";

    response += "}";

    return response;
}

void meshResponse(AsyncWebServerRequest *request)
{
    unsigned long requestStart = millis();
    meshClient.sendBroadcast("INFO?");
    std::list<uint32_t> nodeList = meshClient.getNodeList();
    String connectionJson = subConnectionJson();
    nodeInfo[meshClient.getNodeId()] = getNodeInfo();

    while (
        nodeInfo.size() < nodeList.size() &&
        !TimeUtils.millisSince(requestStart) > 1000)
    {
        yeild();
        delay(10);
    }

    std::string json = "{\"connections\":";
    json += connectionJson.c_str();
    json += ",\"info\":{";

    for (auto &it : nodeInfo)
    {
        json += "\"";
        json += it.first;
        json += "\":";
        json += it.second;
        json += ",";
    }

    json.pop_back();
    json += "}}";
    request->send(200, "application/json", json);
    nodeInfo = {};
    json = "";
};

void AsyncMeshClass::_messageReceivedCallback(const uint32_t &from, const String &message)
{
    Log.infof("Mesh message received from %u message = '%s'\n", from, message.c_str());
    if (message.startsWith("INFO?"))
    {
        std::string response = "INFO>";
        response += getNodeInfo();
        meshClient.sendSingle(from, response.c_str());
        return;
    }
    if (message.startsWith("INFO?"))
    {
        std::string _message = message.c_str();
        _message.replace(0, 5, "");
        nodeInfo[from] = _message;
        return;
    }
};

void AsyncMeshClass::_newConnectionCallback(uint32_t nodeId)
{
    Log.infof("New mesh connection, nodeId = %u\n", nodeId);
};

void AsyncMeshClass::_changedConnectionCallback()
{
    Log.infof("Mesh changed connections\n");
};

void AsyncMeshClass::_nodeTimeAdjustedCallback(int32_t offset)
{
    Log.infof("Adjusted mesh time %u. Offset = %d\n", mesh.getNodeTime(), offset);
};

void AsyncMeshClass::_nodeDelayReceived(uint32_t nodeId, int32_t delay)
{
    Log.infof("Mesh delay from node %u = %d\n", nodeId, delay);
};