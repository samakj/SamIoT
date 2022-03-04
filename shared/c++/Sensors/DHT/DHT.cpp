#include "DHT.h"

SamIoT::Sensors::DHT::DHT(
    uint8_t _pinNo,
    uint8_t _type,
    TemperatureCallback _temperatureCallback,
    HumidityCallback _humidityCallback) : pinNo(_pinNo),
                                          type(_type),
                                          temperatureCallback(_temperatureCallback),
                                          humidityCallback(_humidityCallback){};

void SamIoT::Sensors::DHT::setTemperatureCallback(
    TemperatureCallback _temperatureCallback)
{
    this->temperatureCallback = _temperatureCallback;
};

void SamIoT::Sensors::DHT::setHumidityCallback(
    HumidityCallback _humidityCallback)
{
    this->humidityCallback = _humidityCallback;
};

void SamIoT::Sensors::DHT::setup()
{
    this->client = new _DHT(this->pinNo, this->type);
    this->client->begin();
    SamIoT::Logger::infof("DHT sensor initialised on pin %d\n", this->pinNo);
};

void SamIoT::Sensors::DHT::loop()
{
    if (!this->client)
        this->setup();
    if (SamIoT::Time::millisSince(this->lastReadMillis) > (long unsigned int)this->readPeriod)
    {
        this->checkTemperature();
        this->checkHumidity();
        this->lastReadMillis = millis();
        this->readCount++;
    }
};

void SamIoT::Sensors::DHT::checkTemperature()
{
    float newTemperature = this->client->readTemperature();

    if (!isnan(newTemperature))
    {
        if (abs(newTemperature - temperature) > 0.15)
        {
            if (this->temperatureCallback)
                this->temperatureCallback(newTemperature);
            this->temperature = newTemperature;
        }
    }
    else
        SamIoT::Logger::debug("NaN value returned for DHT temperature");
};

void SamIoT::Sensors::DHT::checkHumidity()
{
    float newHumidity = client->readHumidity();

    if (!isnan(newHumidity))
    {
        if (abs(newHumidity - humidity) > 0.25)
        {
            if (this->humidityCallback)
                this->humidityCallback(newHumidity);
            this->humidity = newHumidity;
        }
    }
    else
        SamIoT::Logger::debug("NaN value returned for DHT humidity");
}