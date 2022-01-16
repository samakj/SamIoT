#include <AsyncDHT.h>

AsyncDHT::AsyncDHT(
    int _pinNo,
    int _type,
    TemperatureCallback _temperatureCallback,
    HumidityCallback _humidityCallback
)
{
    pinNo = _pinNo;
    type = _type;
    temperatureCallback = _temperatureCallback;
    humidityCallback = _humidityCallback;
};

void AsyncDHT::setTemperatureCallback(
    TemperatureCallback _temperatureCallback
)
{
    temperatureCallback = _temperatureCallback;
};

void AsyncDHT::setHumidityCallback(
    HumidityCallback _humidityCallback
)
{
    humidityCallback = _humidityCallback;
};

void AsyncDHT::setup()
{
    client = new DHT(pinNo, type);
    client->begin();     
    Log.infof("DHT sensor initialised on pin %d\n", pinNo);
};

void AsyncDHT::loop()
{
    if (!client) setup();
    if (TimeUtils.millisSince(lastReadMillis) > (long unsigned int)readPeriod)
    {
        checkTemperature();
        checkHumidity();
        lastReadMillis = millis();
        readCount++;
    }
};

void AsyncDHT::checkTemperature()
{
    float newTemperature = client->readTemperature();
    
    if (!isnan(newTemperature)) 
    {
        temperature = newTemperature;
        if (temperatureCallback != nullptr) temperatureCallback(temperature);
    }
};

void AsyncDHT::checkHumidity()
{
    float newHumidity = client->readHumidity();

    if (!isnan(newHumidity)) 
    {
        humidity = newHumidity;
        if (humidityCallback != nullptr) humidityCallback(humidity);
    }
}