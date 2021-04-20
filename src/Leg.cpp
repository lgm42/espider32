#include <ESP32Servo.h> 

#include "Leg.h"

Leg::Leg(const int haunchGpio, const int kneeGpio, const int ankleGpio, const bool haunchClockWize, const bool kneeClockWize, const bool ankleClockWize)
    : _haunch(haunchGpio, haunchClockWize), _knee(kneeGpio, kneeClockWize), _ankle(ankleGpio, ankleClockWize)
{
}

Leg::~Leg()
{
}

void Leg::handle()
{
    _haunch.handle();
    _knee.handle();
    _ankle.handle();
}

AdvancedServo & Leg::haunch()
{
    return _haunch;
}

AdvancedServo & Leg::knee()
{
    return _knee;
}

AdvancedServo & Leg::ankle()
{
    return _ankle;
}

void Leg::move(const int haunchValue, const int kneeValue, const int ankleValue)
{
    _haunch.position(haunchValue);
    _knee.position(kneeValue);
    _ankle.position(ankleValue);
}