#include <ESP32Servo.h> 

#include "MovementManager.h"

#define KNEE_UP         0
#define KNEE_MIDDLE     90
#define KNEE_DOWN       180

#define ANKLE_OUT       180
#define ANKLE_MIDDLE    90
#define ANKLE_IN        0

#define HAUNCH_FRONT    120
#define HAUNCH_MIDDLE   90
#define HAUNCH_BACK     40

#define MOVE_SLEEP      200


const int MovementManager::LegWalkOrder[] = {0, 2, 1, 3};

MovementManager::MovementManager()
{
}

MovementManager::~MovementManager()
{
}

void MovementManager::setup()
{
    // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    _legs[0] = new Leg(19, 33, 32, false, false, false);
    _legs[1] = new Leg(4, 16, 14, false, true, true);
    _legs[2] = new Leg(15, 12, 25, true, false, false);
    _legs[3] = new Leg(17, 26, 27, true, true, true);

    //TODO

    for (int i = 0; i < NUMBER_OF_LEGS; ++i)
    {
        _legs[i]->haunch().position(90);
        _legs[i]->knee().position(90);
        _legs[i]->ankle().position(90);
    }
}

Leg ** MovementManager::legs()
{
    return _legs;
}

void MovementManager::walkFront()
{
    for (int i = 0; i < NUMBER_OF_LEGS; ++i)
    {
        _legs[LegWalkOrder[i]]->knee().position(KNEE_UP);
        _legs[LegWalkOrder[i]]->ankle().position(ANKLE_IN);
        delay(MOVE_SLEEP);

        _legs[LegWalkOrder[i]]->haunch().position(HAUNCH_FRONT);
        delay(MOVE_SLEEP);

        _legs[LegWalkOrder[i]]->knee().position(KNEE_DOWN);
        _legs[LegWalkOrder[i]]->ankle().position(ANKLE_OUT);
        delay(MOVE_SLEEP);
    }

    //go front with all haunches
    for (int i = 0; i < NUMBER_OF_LEGS; ++i)
    {
        _legs[i]->haunch().position(HAUNCH_BACK);
    }
}

void MovementManager::handle()
{
    for (int i = 0; i < NUMBER_OF_LEGS; ++i)
    {
        _legs[i]->haunch().handle();
        _legs[i]->knee().handle();
        _legs[i]->ankle().handle();
    }
}

#if !defined(NO_GLOBAL_INSTANCES)
MovementManager Movement;
#endif

