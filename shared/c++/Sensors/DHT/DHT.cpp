#include "DHT.h"

namespace SamIoT::Sensors
{
    float TEMPERATURE_NULL_VALUE = -1000.0f;
    float HUMIDITY_NULL_VALUE = -1000.0f;

    DHT::DHT(
        uint8_t _pinNo,
        uint8_t _type,
        TemperatureCallback _temperatureCallback,
        HumidityCallback _humidityCallback) : pinNo(_pinNo),
                                              type(_type),
                                              temperatureCallback(_temperatureCallback),
                                              humidityCallback(_humidityCallback){};

    void DHT::setTemperatureCallback(
        TemperatureCallback _temperatureCallback)
    {
        this->temperatureCallback = _temperatureCallback;
    };

    void DHT::setHumidityCallback(
        HumidityCallback _humidityCallback)
    {
        this->humidityCallback = _humidityCallback;
    };

    void DHT::setup()
    {
        this->client = new _DHT(this->pinNo, this->type);
        this->client->begin();
        SamIoT::Logger::infof("DHT sensor initialised on pin %d\n", this->pinNo);
    };

    void DHT::loop()
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

    void DHT::checkTemperature()
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
            this->nanTemperatureReported = false;
        }
        else if (!this->nanTemperatureReported)
        {
            SamIoT::Logger::debug("NaN value returned for DHT temperature");
            this->nanTemperatureReported = true;
        }
    };

    void DHT::checkHumidity()
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
            this->nanHumidityReported = false;
        }
        else if (!this->nanHumidityReported)
        {
            SamIoT::Logger::debug("NaN value returned for DHT humidity");
            this->nanHumidityReported = true;
        }
    }
}