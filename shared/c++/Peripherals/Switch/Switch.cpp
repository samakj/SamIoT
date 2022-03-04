#include "Switch.h"

SamIoT::Switch::AsyncSwitch::AsyncSwitch(
    uint8_t _pinNo,
    uint8_t _outPin,
    bool _defaultState,
    SwitchCallback _switchCallback) : pinNo(_pinNo),
                                      outPin(_outPin),
                                      state(_defaultState),
                                      switchCallback(_switchCallback){};

void SamIoT::Switch::AsyncSwitch::setSwitchCallback(
    SwitchCallback _switchCallback)
{
    this->switchCallback = _switchCallback;
};

void SamIoT::Switch::AsyncSwitch::setup()
{
    pinMode(this->pinNo, INPUT_PULLUP);
    if (this->outPin >= 0)
        pinMode(this->outPin, OUTPUT);
    SamIoT::Logger::infof("Switch sensor initialised on pin %d\n", this->pinNo);
};

void SamIoT::Switch::AsyncSwitch::loop()
{
    bool _state = digitalRead(this->pinNo);
    if (_state != this->state)
    {
        this->switchCallback(_state);
        this->state = _state;
    }
};