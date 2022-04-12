#include "Wifi.h"

SamIoT::Wifi::WifiCredentials::WifiCredentials(std::string _ssid, std::string _password)
{
    ssid = _ssid;
    password = _password;
};

std::string SamIoT::Wifi::MACAddressByteArrayToString(byte MACAddressByteArray[6])
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
        MACAddressByteArray[5]);
    return (std::string)buffer;
};

std::string SamIoT::Wifi::getMACAddressString()
{
    byte mac[6];
    WiFi.macAddress(mac);
    return SamIoT::Wifi::MACAddressByteArrayToString(mac);
};

std::string SamIoT::Wifi::IPAddressToString(IPAddress ip)
{
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

std::string SamIoT::Wifi::getLocalIPAddressString()
{
    return SamIoT::Wifi::IPAddressToString(WiFi.localIP());
};

boolean SamIoT::Wifi::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void SamIoT::Wifi::addConnectCallback(ConnectCallback callback)
{
    SamIoT::Wifi::connectCallbacks.push_back(callback);
}

void SamIoT::Wifi::addSsidCallback(SsidCallback callback)
{
    SamIoT::Wifi::ssidCallbacks.push_back(callback);
}

void SamIoT::Wifi::addStrengthCallback(StrengthCallback callback)
{
    SamIoT::Wifi::strengthCallbacks.push_back(callback);
}

float SamIoT::Wifi::getConnectedNetworkStrength()
{
    if (SamIoT::Time::millisSince(lastStrengthUpdate) > (unsigned long)(strengthUpdatePeriod))
    {
        if (!isConnected())
        {
            SamIoT::Logger::error("Not connected to network, cannot get strength.");

            if (SamIoT::Wifi::strength != STRENGTH_NULL_VALUE)
            {
                SamIoT::Wifi::strength = STRENGTH_NULL_VALUE;
            }
            if (SamIoT::Wifi::ssid != SSID_NULL_VALUE)
            {
                SamIoT::Wifi::ssid = SSID_NULL_VALUE;
                for (
                    SamIoT::Wifi::SsidCallback callback :
                    SamIoT::Wifi::ssidCallbacks)
                    callback(SamIoT::Wifi::ssid);
            }

            return SamIoT::Wifi::strength;
        }

        float _strength = (255 + WiFi.RSSI()) / 255.0f;
        if (_strength != SamIoT::Wifi::strength)
        {
            SamIoT::Wifi::strength = _strength;
            for (
                SamIoT::Wifi::StrengthCallback callback :
                SamIoT::Wifi::strengthCallbacks)
                callback(SamIoT::Wifi::strength);
        }
        SamIoT::Wifi::lastStrengthUpdate = millis();
    }

    return SamIoT::Wifi::strength;
}

std::string SamIoT::Wifi::getConnectedNetworkSsid()
{
    std::string _ssid = (std::string)(WiFi.SSID().c_str());

    if (_ssid != SamIoT::Wifi::ssid)
    {
        SamIoT::Wifi::ssid = _ssid;
        for (
            SamIoT::Wifi::SsidCallback callback :
            SamIoT::Wifi::ssidCallbacks)
            callback(SamIoT::Wifi::ssid);
    }

    return SamIoT::Wifi::ssid;
}

float SamIoT::Wifi::getNetworkStrength(std::string ssid)
{
    SamIoT::Logger::debugf("Finding strength of %s network...\n", ssid.c_str());
    SamIoT::Logger::debug("Scanning local networks...");

    uint8_t networkCount = WiFi.scanNetworks();

    SamIoT::Logger::debugf("%d networks found in range.\n", networkCount);

    for (uint8_t i = 0; i < networkCount; i++)
    {
        if (ssid == WiFi.SSID(i).c_str())
        {
            float strength = (255 + WiFi.RSSI(i)) / 255.0f;
            SamIoT::Logger::debugf("'%s' network found with strength %.1f%%.\n\n", ssid.c_str(), strength * 100);
            return strength;
        }
    }

    SamIoT::Logger::warnf("'%s' network not found.\n\n", ssid.c_str());
    return SamIoT::Wifi::STRENGTH_NULL_VALUE;
}

SamIoT::Wifi::WifiCredentials *SamIoT::Wifi::getStrongestNetwork(std::vector<WifiCredentials *> credentials)
{
    if (!credentials.size())
    {
        SamIoT::Logger::error("Empty list passed to getStrongestNetwork");
        return nullptr;
    }

    SamIoT::Logger::infof("Finding strongest of %d networks...\n", credentials.size());
    SamIoT::Logger::debug("Scanning local networks...");

    uint8_t networkCount = WiFi.scanNetworks();

    SamIoT::Logger::debugf("%d networks found in range.\n", networkCount);

    WifiCredentials *strongest = nullptr;
    float strengthOfStrongest = 0;

    for (uint8_t i = 0; i < networkCount; i++)
    {
        for (WifiCredentials *_credential : credentials)
        {
            if (_credential->ssid == WiFi.SSID(i).c_str())
            {
                float strength = (255 + WiFi.RSSI(i)) / 2.55;
                SamIoT::Logger::debugf("    '%s' network found with strength %.1f%%.\n", _credential->ssid.c_str(), strength);
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
        SamIoT::Logger::error("None of the provided credentials matched found local networks.");
    }
    else
    {
        SamIoT::Logger::infof("'%s' found as the strongest.\n", strongest->ssid.c_str());
    }

    return strongest;
}

void SamIoT::Wifi::connect(
    WifiCredentials *credentials,
    std::string hostname,
    uint8_t ipLocation,
    uint16_t maxWait)
{
    if (!SamIoT::Wifi::isConnected())
    {
        SamIoT::Wifi::_connect(credentials, hostname, ipLocation, maxWait);
    };
}

void SamIoT::Wifi::connect(
    std::vector<WifiCredentials *> credentials,
    std::string hostname,
    uint8_t ipLocation,
    uint16_t maxWait)
{
    if (!SamIoT::Wifi::isConnected())
    {
        WifiCredentials *strongest = SamIoT::Wifi::getStrongestNetwork(credentials);
        if (strongest)
            SamIoT::Wifi::_connect(strongest, hostname, ipLocation, maxWait);
    }
}

void SamIoT::Wifi::_connect(
    WifiCredentials *credentials,
    std::string hostname,
    uint8_t ipLocation,
    uint16_t maxWait)
{
    strength = STRENGTH_NULL_VALUE;
    ssid = SSID_NULL_VALUE;

    unsigned long start = millis();
    uint16_t loop = 0;

    if (
        ipLocation != SamIoT::Wifi::IP_LOCATION_NULL_VALUE &&
        !SamIoT::Wifi::isConnected())
    {
        IPAddress local_ip(192, 168, 1, ipLocation);
        IPAddress gateway(192, 168, 1, 1);
        IPAddress subnet(255, 255, 0, 0);
        IPAddress dns1(8, 8, 8, 8);
        IPAddress dns2(4, 4, 4, 4);
        if (!WiFi.config(local_ip, gateway, subnet, dns1, dns2))
        {
            SamIoT::Logger::error("Failed to set IP information.");
            return;
        }
    }
    while (!SamIoT::Wifi::isConnected())
    {
        float timeSinceStart = (millis() - start) / 1000.;

        if (!(loop % 100))
        {
            // WiFi.mode(WIFI_STA);
            WiFi.begin(
                credentials->ssid.c_str(),
                credentials->password.c_str());
        }
        if (timeSinceStart >= maxWait)
        {
            SamIoT::Logger::warn("Max wait exceeded.", "\n");
            return;
        }
        SamIoT::Logger::infof(
            "Attempting to connect to '%s'... %.1fs\r",
            credentials->ssid.c_str(),
            (millis() - start) / 1000.);
        loop++;
        delay(100);
    }

    SamIoT::Wifi::ssid = (std::string)credentials->ssid.c_str();

    std::string ip = getLocalIPAddressString();
    std::string mac = getMACAddressString();

    SamIoT::Logger::info("");
    SamIoT::Logger::infof("Connected to '%s'.\n", ssid.c_str());
    SamIoT::Logger::infof("IP:   %s\n", ip.c_str());
    SamIoT::Logger::infof("MAC:  %s\n", mac.c_str());

    WiFi.setHostname(hostname.c_str());

    for (
        SamIoT::Wifi::ConnectCallback callback :
        SamIoT::Wifi::connectCallbacks)
        callback(SamIoT::Wifi::ssid);
};