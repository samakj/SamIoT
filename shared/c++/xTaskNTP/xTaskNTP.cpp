#include "xTaskDHT.h"

xTaskNTPClass xTaskNTPClass::_instance;

xTaskNTPClass &xTaskNTPClass::getInstance()
{
    return _instance;
}

void xTaskNTPClass::start()
{
    xTaskCreate(
        NTPReconnectTask,
        "NTP Reconnect Task",
        1024,
        this,
        1,
        task);
    Sam::Log.infof("NTP reconnect task started.");
};

void xTaskNTPClass::stop()
{
    if (task != nullptr)
        vTaskDelete(task);
};

bool xTaskNTPClass::isConnected()
{
    return time(nullptr) > 1616000000;
};

void xTaskNTPClass::connect(bool force)
{
    if (!force && isConnected())
        return;

    Sam::Log.infof("Connecting to NTP server at %s...\n", server.c_str());

    if (WiFi.status() != WL_CONNECTED)
    {
        Sam::Log.warn("Wifi not connected, skipping NTP sync.");
        return;
    }

    int start = millis();
    int loop = 0;

    do
    {
        int timeSinceStart = TimeUtils.millisSince(start);

        if (!(loop % 100))
            configTime(timezone * 3600, dst * 3600, server.c_str());
        if (timeSinceStart >= maxWait)
        {
            Sam::Log.error("  Max wait exceeded.", "\n");
            return;
        }
        Sam::Log.infof(
            "Waiting for sync %.1fs...\r",
            server.c_str(),
            (millis() - start) / 1000.);
        loop++;
        delay(100);
    } while (!isConnected())

        Sam::Log.infof(
            "Synced with '%s'. Time: %s.\n",
            server.c_str(),
            TimeUtils.getIsoTimestamp().c_str());

    for (ConnectCallback callback : connectCallbacks)
        callback();
};

void xTaskNTPClass::addConnectCallback(ConnectCallback callback)
{
    connectCallbacks.push_back(callback);
};

void xTaskNTPClass::setServer(std::string _server)
{
    server = _server;
};

void xTaskNTPClass::setTimezone(uint16_t _timezone)
{
    timezone = _timezone;
};

void xTaskNTPClass::setDST(uint8_t _dst)
{
    dst = _dst;
};

void xTaskNTPClass::setMaxWait(uint16_t _maxWait)
{
    maxWait = _maxWait;
};

void xTaskNTPClass::setRefreshPeriod(uint16_t _refreshPeriod)
{
    refreshPeriod = _refreshPeriod;
};

void xTaskNTPClass::DHTUpdateTask(xTaskNTPClass *cls)
{
    while (true)
    {
        cls->connect(true);
        vTaskDelay(cls->refreshPeriod / portTICK_PERIOD_MS);
    }
}