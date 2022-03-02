#include <AsyncSwitch.h>

AsyncSwitch::AsyncSwitch(
    int _pinNo,
    int _outPin,
    bool _defaultState,
    SwitchCallback _switchCallback)
{
    pinNo = _pinNo;
    outPin = _outPin;
    state = _defaultState;
    switchCallback = _switchCallback;
};

void AsyncSwitch::setSwitchCallback(
    SwitchCallback _switchCallback)
{
    switchCallback = _switchCallback;
};

void AsyncSwitch::setup()
{
    pinMode(pinNo, INPUT_PULLUP);
    if (outPin >= 0)
        pinMode(outPin, OUTPUT);
    Sam::Log.infof("Switch sensor initialised on pin %d\n", pinNo);
};

void AsyncSwitch::loop()
{
    boolean _state = digitalRead(pinNo);
    if (_state != state)
    {
        state = _state;
        switchCallback(state);
    }
};