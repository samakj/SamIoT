#ifndef xTaskDHT_h
#define xTaskDHT_h

#include <Arduino.h>
#include <functional>
#include <vector>

#include <DHT.h>

#include <Log.h>

class xTaskDHT
{
public:
    typedef std::function<void(float temperature, float previousTemperature)> TemperatureCallback;
    typedef std::function<void(float humidity, float previousHumidity)> HumidityCallback;

    static float TEMPERATURE_NULL_VALUE = -1000.0f;
    static float HUMIDITY_NULL_VALUE = -1000.0f;
    static int8_t PIN_NUMBER_NULL_VALUE = -1;

    DHT *client = nullptr;
    int8_t pinNo = xTaskDHT::PIN_NUMBER_NULL_VALUE;
    uint8_t type = DHT22;

    std::vector<TemperatureCallbacks> temperatureCallbacks = {};
    std::vector<HumidityCallbacks> humidityCallbacks = {};

    float minTemperatureDifference = 0.15f;
    float minHumidityDifference = 0.25f;

    uint16_t readPeriod = 2000;

private:
    float temperature = xTaskDHT::TEMPERATURE_NULL_VALUE;
    float humidity = xTaskDHT::HUMIDITY_NULL_VALUE;

    uint32_t lastReadMillis = 0;
    uint32_t readCount = 0;

    TaskHandle_t task = nullptr;

public:
    xTaskDHT(
        int8_t pinNo,
        uint8_t type = DHT22,
        uint16_t readPeriod = 2000,
        float minTemperatureDifference = 0.15,
        float minHumidityDifference = 0.25);

    void start();
    void stop();

    float getTemperature();
    float getHumidity();
    uint32_t getLastReadMillis();
    uint32_t getReadCount();

    void addTemperatureCallback(TemperatureCallback callback);
    void addHumidityCallback(HumidityCallback callback);

    void setPinNo(int8_t pinNo);
    void setMinTemperatureDifference(float minTemperatureDifference);
    void setMinHumidityDifference(float minHumidityDifference);
    void setReadPeriod(uint16_t readPeriod);

    void updateTemperature();
    void updateHumidity();

private:
    static void DHTUpdateTask(xTaskDHT *cls);
};

#endif