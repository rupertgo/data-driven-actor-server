#include "ServoBehaviours.h"

ServoGoAndReturn::ServoGoAndReturn(Servo servo, String name, int delay, int pause, int start_angle, int end_angle) :
	Behaviour(name), _servo(servo), _delay(delay), _pause(pause), _start_angle(start_angle), _end_angle(end_angle)
{}

void ServoGoAndReturn::update()
{
	_servo.write(_start_angle);
	delay(_pause);

	for(int i = 0; i < _end_angle; i++)
	{
		_servo.write(i);
		delay(_delay);
	}

	_servo.write(_end_angle);
	delay(_pause);

	for(int i = 180; i >= _start_angle; i--)
	{
		_servo.write(i);
		delay(_delay/2);
	}

	_servo.write(_start_angle);
	_rotations++;

	if( _rotations >= _num_rotations )
	{
		_running = false;
	}
};

String ServoGoAndReturn::start(String args)
{
	_num_rotations = args.toInt();
	_rotations = 0;
	_running = true;
};

char * ServoGoAndReturn::args()
{
	return "<int number_of_cycles>";
};

String ServoGoTo::start(String args)
{
	Serial.println("Goto: '"+args+"'");
	int val = args.toInt();
	_servo.write(val);

	return "";
};
ServoGoTo::ServoGoTo(Servo servo, String name) :  Behaviour(name), _servo(servo)
{}

char * ServoGoTo::args()
{
	return "<int angle>";
};

void ServoWiggle::update()
{
	int time_since = millis() - _start_time;

	if( time_since > _wiggle_time )
	{
		_wiggles++;
		_start_time = millis();
		int angle = ( _wiggles % 2 ) ? (90+_wiggle_angle) : (90-_wiggle_angle);

		if( _wiggles > _num_wiggles )
		{
			angle = 90;
			_running = false;
		}

		Serial.println("Wiggling to: " + String(angle));
		_servo.write(angle);
	}
};

ServoWiggle::ServoWiggle(Servo servo, String name, int slowness) : Behaviour(name), _servo(servo),_wiggle_factor(slowness)
{}

String ServoWiggle::start(String args)
{
	_wiggle_angle = args.toInt();
	_wiggles = 0;
	_running = true;
	_wiggle_time = _wiggle_factor * _wiggle_angle;

	return "Wiggling " + String(_num_wiggles) + " times";
};

char * ServoWiggle::args()
{
	return "<int wiggle_angle>";
};
