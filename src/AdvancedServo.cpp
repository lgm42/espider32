#include "AdvancedServo.h"

#define MAX_MILLIS_TO_RELEASE 500

AdvancedServo::AdvancedServo(const int gpio, const bool clockwize, const int initialValue)
: _gpio(gpio), _clockwize(clockwize)
{
    _servo.setPeriodHertz(50);  // Standard 50hz servo
    position(initialValue);
}

AdvancedServo::~AdvancedServo()
{
}

void AdvancedServo::position(const int value)
{
    _servo.attach(_gpio, 500, 2400);   // using SG90 servo min/max of 500us and 2400us
                                        
    if (_clockwize)
        _servo.write(value);
    else
        _servo.write(180 - value);
    _lastCommandMillis = millis();
}

int AdvancedServo::position()
{
    if (_clockwize)
        return _servo.read();
    return 180 - _servo.read();
}

void AdvancedServo::handle()
{
    if (_lastCommandMillis > 0)
    {
        //servo is attached
        if (millis() - _lastCommandMillis > MAX_MILLIS_TO_RELEASE)
        {
            //we can release 
            _servo.detach();
            pinMode(_gpio, INPUT);
            _lastCommandMillis = 0;
        }
    }
}