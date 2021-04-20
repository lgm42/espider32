#pragma once

#include "ESP32Servo.h"

class AdvancedServo
{
    public:
        AdvancedServo(const int gpio, const bool clockwize, const int initialValue = 90);
        virtual ~AdvancedServo();

        void position(const int value);
        int position();
        void handle();
    private:
        int _gpio;
        bool _clockwize;
        unsigned long _lastCommandMillis;
        Servo _servo;
};