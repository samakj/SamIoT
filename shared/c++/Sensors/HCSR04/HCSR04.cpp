#include "HCSR04.h"

namespace SamIoT::Sensors
{
    float DISTANCE_NULL_VALUE = -1000.0f;

    HCSR04::HCSR04(
        uint8_t _triggerPin,
        uint8_t _echoPin,
        DistanceCallback _distanceCallback) : triggerPin(_triggerPin),
                                              echoPin(_echoPin),
                                              distanceCallback(_distanceCallback){};

    void HCSR04::setDistanceCallback(
        DistanceCallback _distanceCallback)
    {
        this->distanceCallback = _distanceCallback;
    };

    void HCSR04::setTemperature(float _temperature)
    {
        this->temperature = _temperature;
    }

    float HCSR04::getSpeedOfSound()
    {
        return 331.3 + 0.606 * this->temperature;
    }

    void HCSR04::setup()
    {

        pinMode(this->triggerPin, OUTPUT);
        pinMode(this->echoPin, INPUT);
        SamIoT::Logger::infof("HCSR04 sensor initialised on pin %d & %d\n", this->triggerPin, this->echoPin);
    };

    void HCSR04::loop()
    {
        if (SamIoT::Time::millisSince(this->lastReadMillis) > this->readPeriod)
        {
            this->check();
            this->lastReadMillis = millis();
            this->readCount++;
        }
    };

    void HCSR04::check()
    {
        float newDistance = this->getMedianDistance();

        if (!isnan(newDistance) && newDistance > 0 && newDistance < 3 && abs(distance - newDistance) > 0.005f)
        {
            if (this->distanceCallback != nullptr)
                this->distanceCallback(newDistance);
            this->distance = newDistance;
        }
        else if (newDistance <= 0 || newDistance >= 3)
        {
            SamIoT::Logger::debugf("Anomylous Distance of %.3fm reported...\n", newDistance);
        }
    };

    void HCSR04::trigger()
    {
        pinMode(this->triggerPin, OUTPUT);
        digitalWrite(this->triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(this->triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(this->triggerPin, LOW);
    }

    float HCSR04::getEchoTime()
    {
        pinMode(this->echoPin, INPUT);
        return pulseIn(this->echoPin, HIGH, 50000) / 1000000.0f;
    }

    float HCSR04::getDistance()
    {
        this->trigger();
        float duration = this->getEchoTime();
        return duration * this->getSpeedOfSound() / 2;
    }

    float HCSR04::getMedianDistance(uint8_t noTests)
    {
        std::vector<float> values = {};

        for (uint8_t i = 0; i < noTests; i++)
        {
            float _distance = this->getDistance();

            if (_distance > 0 && _distance < 3)
            {
                values.push_back(_distance);
            }
            delayMicroseconds(100);
        }

        if (!values.size())
            return SamIoT::Sensors::DISTANCE_NULL_VALUE;

        size_t n = values.size() / 2;
        nth_element(values.begin(), values.begin() + n, values.end());
        return values[n];
    }
}