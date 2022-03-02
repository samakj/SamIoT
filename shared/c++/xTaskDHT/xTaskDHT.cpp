#include "xTaskDHT.h"

xTaskDHT::xTaskDHT(
    int8_t _pin,
    uint8_t _type,
    uint16_t _readPeriod,
    float _minTemperatureDifference,
    float _minHumidityDifference) : pin(_pin),
                                    type(_type),
                                    readPeriod(_readPeriod),
                                    minTemperatureDifference(_minTemperatureDifference),
                                    minHumidityDifference(_minHumidityDifference){};

void xTaskDHT::start()
{
    if (pinNo == xTaskDHT::PIN_NUMBER_NULL_VALUE)
    {
        Sam::Log.error("DHT sensor pin not set, ignoring.");
        return
    }

    client = new DHT(pinNo, type);
    client->begin();
    client->begin();
    Sam::Log.infof("DHT sensor initialised on pin %d.\n", pinNo);

    char taskName[64];
    sprintf("DHT Update Task, Pin %d", pinNo);
    xTaskCreate(
        DHTUpdateTask,
        taskName,
        1024,
        this,
        1,
        task);
    Sam::Log.infof("DHT sensor task on pin %d started.\n", pinNo);
};

void xTaskDHT::stop()
{
    if (task != nullptr)
        vTaskDelete(task);
};

float xTaskDHT::getTemperature()
{
    return temperature;
};

float xTaskDHT::getHumidity()
{
    return humidity;
};

uint32_t xTaskDHT::getLastReadMillis()
{
    return lastReadMillis;
};

uint32_t xTaskDHT::getReadCount()
{
    return readCount;
};

void xTaskDHT::addTemperatureCallback(TemperatureCallback callback)
{
    temperateCallbacks.push_back(callback);
};

void xTaskDHT::addHumidityCallback(HumidityCallback callback)
{
    humidityCallbacks.push_back(callback);
};

void xTaskDHT::setPinNo(int8_t _pinNo)
{
    pinNo = _pinNo;
};

void xTaskDHT::setMinTemperatureDifference(float _minTemperatureDifference)
{
    minTemperatureDifference = _minTemperatureDifference;
};

void xTaskDHT::setMinHumidityDifference(float _minHumidityDifference)
{
    minHumidityDifference = _minHumidityDifference;
};

void xTaskDHT::setReadPeriod(uint16_t _readPeriod)
{
    readPeriod = _readPeriod;
};

void xTaskDHT::updateTemperature()
{
    float newTemperature = client->readTemperature();

    if (isnan(newTemperature))
        Sam::Log.warnf("DHT sensor on pin %d reported NaN temperature.\n", pinNo);
    else if (abs(newTemperature - temperature) > minTemperatureDifference)
    {
        for (TemperatureCallback callback : temperatureCallbacks)
            callback(newTemperature, temperature);

        Sam::Log.infof("Temperature on pin %d changed from %.1f to %.1f", pinNo, temperature, newTemperature);
        temperature = newTemperature;
    }
};

void xTaskDHT::updateHumidity()
{
    float newHumidity = client->readHumidity();

    if (isnan(newHumidity))
        Sam::Log.warnf("DHT sensor on pin %d reported NaN humidity.\n", pinNo);
    else if (abs(newHumidity - humidity) > minHumidityDifference)
    {
        for (HumidityCallback callback : humidityCallbacks)
            callback(newHumidity, humidity);

        Sam::Log.infof("Humidity on pin %d changed from %.1f to %.1f", pinNo, humidity, newHumidity);
        humidity = newHumidity;
    }
};

void xTaskDHT::DHTUpdateTask(xTaskDHT *cls)
{
    while (true)
    {
        cls->updateTemperature();
        cls->updateHumidity();
        vTaskDelay(cls->readPeriod / portTICK_PERIOD_MS);
    }
}