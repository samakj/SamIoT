#ifndef xTaskNTP_h
#define xTaskNTP_h

#include <Arduino.h>
#include <functional>
#include <string>
#include <time.h>
#include <vector>

#include <Log.h>
#include <TimeUtils.h>

class xTaskNTPClass
{
public:
    typedef std::function<void()> ConnectCallback;

    xTaskNTPClass(const xTaskNTPClass &) = delete;
    static xTaskNTPClass &getInstance();

    std::string server = "uk.pool.ntp.org";
    uint16_t timezone = 0;
    uint8_t dst = 0;
    uint16_t maxWait = 60000;
    uint16_t refreshPeriod = 3600000;

    std::vector<ConnectCallback> connectCallbacks;

private:
    unsigned long lastReconnect = 0;

    TaskHandle_t task = nullptr;

public:
    void start();
    void stop();

    bool isConnected();
    void connect(bool force = false);

    void addConnectCallback(ConnectCallback callback);

    void setServer(std::string server);
    void setTimezone(int16_t timezone);
    void setDST(uint8_t dst);
    void setMaxWait(uint16_t maxWait);
    void setRefreshPeriod(uint16_t refreshPeriod);

private:
    xTaskNTPClass(){};
    static xTaskNTPClass _instance;

    static void NTPReconnectTask(xTaskNTPClass *cls);
};

static xTaskNTPClass &xTaskNTP = xTaskNTPClass::getInstance();

#endif