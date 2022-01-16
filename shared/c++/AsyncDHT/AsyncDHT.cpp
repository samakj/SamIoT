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
    _temperaturePurgatory = new ValuePurgatory<float>(
        TEMPERATURE_NULL_VALUE,
        TEMPERATURE_NULL_VALUE,
        10,
        0,
        10,
        0.25,
        [this](float newValue, float oldValue){
            if (temperatureCallback != nullptr) temperatureCallback(newValue);
            temperature = newValue;
        }
    );
    _humidityPurgatory = new ValuePurgatory<float>(
        HUMIDITY_NULL_VALUE,
        HUMIDITY_NULL_VALUE,
        10,
        0,
        10,
        0.5,
        [this](float newValue, float oldValue){
            if (humidityCallback != nullptr) humidityCallback(newValue);
            humidity = newValue;
        }
    );
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
        if (_temperaturePurgatory->isRunningIn() && !(readCount % 5))
        {
            Log.debugf(
                "DHT sensor running in, %s remaining...\n",
                _temperaturePurgatory->runInInfo().c_str()
            );
        }
        _temperaturePurgatory->addValue(newTemperature);
    }
};

void AsyncDHT::checkHumidity()
{
    float newHumidity = client->readHumidity();

    if (!isnan(newHumidity)) 
    {
        _humidityPurgatory->addValue(newHumidity);
    }
}