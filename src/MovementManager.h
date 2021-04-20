#pragma once

#include <ESP32Servo.h> 

#include "contants.h"
#include "Leg.h"

class MovementManager
{
    public:
        MovementManager();
        virtual ~MovementManager();

        void setup();

        Leg ** legs();

        void walkFront();

        void handle();

    private:
        Leg * _legs[NUMBER_OF_LEGS];
        static const int LegWalkOrder[];
};

#if !defined(NO_GLOBAL_INSTANCES)
extern MovementManager Movement;
#endif
