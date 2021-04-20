#pragma once

#include "AdvancedServo.h"

class Leg
{
    public:
        Leg(const int haunchGpio, const int kneeGpio, const int ankleGpio, const bool haunchClockWize, const bool kneeClockWize, const bool ankleClockWize);
        virtual ~Leg();

        void move(const int haunchValue, const int kneeValue, const int ankleValue);

        AdvancedServo & haunch();
        AdvancedServo & knee();
        AdvancedServo & ankle();

        void handle();
    private:
        AdvancedServo _haunch;
        AdvancedServo _knee;
        AdvancedServo _ankle;
};