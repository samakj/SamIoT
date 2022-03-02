#include <AsyncNTP.h>

AsyncNTPClass AsyncNTPClass::_instance;

AsyncNTPClass &AsyncNTPClass::getInstance()
{
    return _instance;
}

bool AsyncNTPClass::isConnected()
{
    return time(nullptr) > 1616000000;
};

void AsyncNTPClass::addConnectCallback(ConnectCallback callback)
{
    connectCallbacks.push_back(callback);
}

void AsyncNTPClass::addWifiConnectCallback()
{
    usingWifiCallback = true;
    Sam::Log.debug("Adding NTP Wifi connect callback.");
    AsyncWifi.addConnectCallback(
        [this](std::string ssid)
        { connect(); });
}

void AsyncNTPClass::connect()
{
    if (!isConnected())
    {
        Sam::Log.infof("NTP not connected, connecting to %s..\n", server.c_str());

        if (!AsyncWifi.isConnected())
        {
            Sam::Log.warn("Wifi not connected, skipping NTP sync.");
            return;
        }

        int start = millis();
        int loop = 0;

        while (!isConnected())
        {
            int timeSinceStart = (millis() - start) / 1000.;

            if (!(loop % 100))
            {
                configTime(timezone * 3600, dst * 3600, server.c_str());
            }
            if (timeSinceStart >= maxWait)
            {
                Sam::Log.error("Max wait exceeded.", "\n");
                return;
            }
            Sam::Log.infof(
                "Attempting to sync with '%s'... %.1fs\r",
                server.c_str(),
                (millis() - start) / 1000.);
            loop++;
            delay(100);
        }

        Sam::Log.info("");
        Sam::Log.infof(
            "Synced with '%s'. Time: %s.\n",
            server.c_str(),
            TimeUtils.getIsoTimestamp().c_str());

        for (ConnectCallback callback : connectCallbacks)
            callback();
    }
};

void AsyncNTPClass::loop()
{
    if (!usingWifiCallback)
    {
        connect();
    }
    if (TimeUtils.millisSince(lastReconnect) > 1000 * 60 * 10)
    {
        connect();
        lastReconnect = millis();
    }
};

void AsyncNTPClass::setServer(std::string _server)
{
    server = _server;
};

void AsyncNTPClass::setTimezone(int _timezone)
{
    timezone = _timezone;
};

void AsyncNTPClass::setDST(int _dst)
{
    dst = _dst;
};

void AsyncNTPClass::setMaxWait(int _maxWait)
{
    maxWait = _maxWait;
};
