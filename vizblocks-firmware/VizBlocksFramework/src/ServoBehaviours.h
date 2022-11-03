#ifndef SERVO_BEHAVIOUR_h
#define SERVO_BEHAVIOUR_h
#include <Arduino.h>
#include "Behaviours.h"
#include <Servo.h>

/** \file ServoBehaviours.h
    \brief Behaviours to control a servo motor.
		This should be extended with stable behaviours that are developed in the
		future.
*/

/** \class ServoGoTo
    \brief Behaviour that sets a servo to an angle specified as its argument.
*/
class ServoGoTo : public Behaviour
{
	Servo _servo;

public:
	ServoGoTo(Servo servo, String name = "GoTo");
	char* args();
	String start(String args);
};

/** \class ServoWiggle
    \brief Behaviour that wiggles a servo with the sweep angle specified as its argument.
*/
class ServoWiggle : public Behaviour
{
	Servo _servo;
	int _start_time = 0;
	int _wiggle_time = 300;
	int _num_wiggles = 5;
	int _wiggles = 0;
	int _wiggle_angle = 0;
	//Calculate wiggle time by multiplying the angle by this...
	int _wiggle_factor = 5;

public:
	ServoWiggle(Servo servo, String name = "Wiggle", int slowness=3);
	char* args();
	String start(String args);
	void update();

};

/** \class ServoGoAndReturn
    \brief Behaviour that sets a servo to an angle before returning its position
		back to zero, for a number of cycles specified as its argument.
*/
class ServoGoAndReturn : public Behaviour
{
	Servo _servo;
	int _start_angle = 0;
	int _end_angle = 180;
	int _delay = 30;
	int _num_rotations = 1;
	int _rotations = 0;
	int _pause = 500;

public:
	ServoGoAndReturn(Servo servo, String name="GoAndReturn", int delay=30, int pause=500, int start_angle = 2, int end_angle=178 );
	char* args();
	String start(String args);
	void update();
};

#endif
