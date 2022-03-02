#include <AsyncHCSR04.h>

AsyncHCSR04::AsyncHCSR04(
    int _triggerPin,
    int _echoPin,
    DistanceCallback _distanceCallback
)
{
    triggerPin = _triggerPin;
    echoPin = _echoPin;
    distanceCallback = _distanceCallback;
};

void AsyncHCSR04::setDistanceCallback(
    DistanceCallback _distanceCallback
)
{
    distanceCallback = _distanceCallback;
};

void AsyncHCSR04::setTemperature(float _temperature)
{
    temperature = _temperature;
}

float AsyncHCSR04::getSpeedOfSound()
{
    return 331.3 + 0.606 * temperature;
}

void AsyncHCSR04::setup()
{

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    _distancePurgatory = new ValuePurgatory<float>(
        DISTANCE_NULL_VALUE,
        DISTANCE_NULL_VALUE,
        10,
        0,
        10,
        0.009,
        [this](float newValue, float oldValue){
            if (distanceCallback != nullptr) distanceCallback(newValue);
            distance = newValue;
        }
    );
    Log.infof("HCSR04 sensor initialised on pin %d & %d\n", triggerPin, echoPin);
};

void AsyncHCSR04::loop()
{
    if (_distancePurgatory == nullptr) setup();
    if (TimeUtils.millisSince(lastReadMillis) > readPeriod)
    {
        check();
        lastReadMillis = millis();
        readCount++;
    }
};

void AsyncHCSR04::check()
{
    if (_distancePurgatory->isRunningIn())
    {
        if (!(readCount % 5))
        {
            Log.debugf(
                "HCSR04 sensor running in, %s remaining...\n",
                _distancePurgatory->runInInfo().c_str()
            );
        }
    }

    float newDistance = getMedianDistance();

    if (!isnan(newDistance) && newDistance > 0 && newDistance < 3)
    {
        _distancePurgatory->addValue(newDistance);
    }
    else if (newDistance <= 0 || newDistance >= 3)
    {
        Log.debugf("Anomylous Distance of %.3fm reported...\n", newDistance);
    }
};

void AsyncHCSR04::trigger()
{
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
}

float AsyncHCSR04::getEchoTime()
{
    pinMode(echoPin, INPUT);
    return pulseIn(echoPin, HIGH) / 1000000.0f;
}

float AsyncHCSR04::getDistance()
{
    trigger();
    float duration = getEchoTime();
    return duration * getSpeedOfSound() / 2;
}

float AsyncHCSR04::getMedianDistance(int noTests)
{
    std::vector<float> values = {};

    for (int i = 0; i < noTests; i++)
    {
        values.push_back(getDistance());
        delayMicroseconds(100);
    }

    size_t n = values.size() / 2;
    nth_element(values.begin(), values.begin() + n, values.end());
    return values[n];
}