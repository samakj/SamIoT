#include <AsyncWifi.h>

WifiCredentials::WifiCredentials(std::string _ssid, std::string _password)
{
    ssid = _ssid;
    password = _password;
};

AsyncWifiClass AsyncWifiClass::_instance;

AsyncWifiClass& AsyncWifiClass::getInstance()
{
    return _instance;
}

std::string AsyncWifiClass::MACAddressByteArrayToString(byte MACAddressByteArray[6])
{
    char buffer[32];
    sprintf(
        buffer,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        MACAddressByteArray[0],
        MACAddressByteArray[1],
        MACAddressByteArray[2],
        MACAddressByteArray[3],
        MACAddressByteArray[4],
        MACAddressByteArray[5]
    );
    return (std::string)buffer;
};

std::string AsyncWifiClass::getMACAddressString()
{
    byte mac[6];
    WiFi.macAddress(mac);
    return MACAddressByteArrayToString(mac);
};

std::string AsyncWifiClass::IPAddressToString(IPAddress ip)
{
    char buffer[32];
    sprintf(
        buffer,
        "%d.%d.%d.%d",
        ip[0],
        ip[1],
        ip[2],
        ip[3]
    );
    return (std::string)buffer;
};

std::string AsyncWifiClass::getLocalIPAddressString()
{
    return IPAddressToString(WiFi.localIP());
};

boolean AsyncWifiClass::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void AsyncWifiClass::addConnectCallback(ConnectCallback callback)
{
    connectCallbacks.push_back(callback);
}

void AsyncWifiClass::addSsidCallback(SsidCallback callback)
{
    ssidCallbacks.push_back(callback);
}

void AsyncWifiClass::addStrengthCallback(StrengthCallback callback)
{
    strengthCallbacks.push_back(callback);
}

float AsyncWifiClass::getConnectedNetworkStrength()
{
    if (TimeUtils.millisSince(_lastStrengthUpdate) > (unsigned long)(strengthUpdatePeriod * 1000))
    {
        if (!isConnected()) 
        {
            Log.error("Not connected to network, cannot get strength.");
            
            if (strength != STRENGTH_NULL_VALUE) {
                strength = STRENGTH_NULL_VALUE;
            }
            if (ssid != SSID_NULL_VALUE) {
                ssid = SSID_NULL_VALUE;
                for (SsidCallback callback : ssidCallbacks) callback(ssid);
            }
            
            return strength;
        }

        float _strength = (255 + WiFi.RSSI()) / 255.0f;
        if (_strength != strength)
        {
            strength = _strength;
            for (StrengthCallback callback : strengthCallbacks) callback(strength);
        }
        _lastStrengthUpdate = millis();
    }

    return strength;
}

std::string AsyncWifiClass::getConnectedNetworkSsid()
{
    std::string _ssid = (std::string)(WiFi.SSID().c_str());

    if (_ssid != ssid)
    {
        ssid = _ssid;
        for (SsidCallback callback : ssidCallbacks) callback(ssid);
    }

    return ssid;
}

float AsyncWifiClass::getNetworkStrength(std::string ssid)
{
    Log.debugf("Finding strength of %s network...\n", ssid.c_str());
    Log.debug("Scanning local networks...");

    int networkCount = WiFi.scanNetworks();

    Log.debugf("%d networks found in range.\n", networkCount);

    for (int i = 0; i < networkCount; i++) {
        if (ssid == WiFi.SSID(i).c_str())
        {
            float strength = (255 + WiFi.RSSI(i)) / 255.0f;
            Log.debugf("'%s' network found with strength %.1f%%.\n\n", ssid.c_str(), strength * 100);
            return strength;
        }
    }

    Log.warnf("'%s' network not found.\n\n", ssid.c_str());
    return STRENGTH_NULL_VALUE;
}

WifiCredentials* AsyncWifiClass::getStrongestNetwork(std::vector<WifiCredentials*> credentials)
{
    if (!credentials.size())
    {
        Log.error("Empty list passed to getStrongestNetwork");
        return nullptr;
    }

    Log.infof("Finding strongest of %d networks...\n", credentials.size());
    Log.debug("Scanning local networks...");

    int networkCount = WiFi.scanNetworks();

    Log.debugf("%d networks found in range.\n", networkCount);

    WifiCredentials* strongest = nullptr;
    int strengthOfStrongest = 0;

    for (int i = 0; i < networkCount; i++) {
        for (WifiCredentials* _credential : credentials)
        {
            if (_credential->ssid == WiFi.SSID(i).c_str())
            {
                float strength = (255 + WiFi.RSSI(i)) / 2.55;
                Log.debugf("    '%s' network found with strength %.1f%%.\n", _credential->ssid.c_str(), strength);
                if (strength > strengthOfStrongest)
                {
                    strongest = _credential;
                    strengthOfStrongest = strength;
                }
            }
        }
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
}

void AsyncWifiClass::connect(
    WifiCredentials* credentials,
    std::string hostname,
    int ipLocation,
    int maxWait
)
{
    if (!isConnected())
    {
        _connect(credentials, hostname, ipLocation, maxWait);
    };
}

void AsyncWifiClass::connect(
    std::vector<WifiCredentials*> credentials,
    std::string hostname,
    int ipLocation,
    int maxWait
)
{
    if (!isConnected())
    {
        WifiCredentials* strongest = getStrongestNetwork(credentials);
        if (strongest) _connect(strongest, hostname, ipLocation, maxWait);
    }
}

void AsyncWifiClass::_connect(
    WifiCredentials* credentials,
    std::string hostname,
    int ipLocation,
    int maxWait
)
{
    strength = STRENGTH_NULL_VALUE;
    ssid = SSID_NULL_VALUE;

    int start = millis();
    int loop = 0;

    if (ipLocation != IP_LOCATION_NULL_VALUE && !isConnected()) 
    {
        IPAddress local_ip(192, 168, 1, ipLocation);
        IPAddress gateway(192, 168, 1, 1);
        IPAddress subnet(255, 255, 0, 0);
        IPAddress dns1(8, 8, 8, 8);
        IPAddress dns2(4, 4, 4, 4);
        if (!WiFi.config(local_ip, gateway, subnet, dns1, dns2))
        {
            Log.error("Failed to set IP information.");
            return;
        }
    }
    while (!isConnected())
    {
        int timeSinceStart = (millis() - start) / 1000.;

        if (!(loop % 100))
        {
            // WiFi.mode(WIFI_STA);
            WiFi.begin(
                credentials->ssid.c_str(),
                credentials->password.c_str()
            );
        }
        if (timeSinceStart >= maxWait)
        {
            Log.warn("Max wait exceeded.", "\n");
            return;
        }
        Log.infof(
            "Attempting to connect to '%s'... %.1fs\r",
            credentials->ssid.c_str(),
            (millis() - start) / 1000.
        );
        loop++;
        delay(100);
    }

    ssid = (std::string)credentials->ssid.c_str();

    std::string ip = getLocalIPAddressString();
    std::string mac = getMACAddressString();
    
    Log.info("");
    Log.infof("Connected to '%s'.\n", ssid.c_str());
    Log.infof("IP:   %s\n", ip.c_str());
    Log.infof("MAC:  %s\n", mac.c_str());

    WiFi.setHostname(hostname.c_str());

    for (ConnectCallback callback : connectCallbacks) callback(ssid);
};