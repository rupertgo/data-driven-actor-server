#ifndef LED_BEHAVIOUR_h
#define LED_BEHAVIOUR_h
#include <Arduino.h>
#include "Behaviours.h"

#include <Adafruit_NeoPixel.h>

/** \file LEDBehaviours.h
    \brief Behaviours to control a neopixel ring.
		This should be extended with stable behaviours that are developed in the
		future.
*/

/** \class LightSomeLEDs
    \brief Behaviour that lights up a number of LEDs specified as its argument.
*/
class LightSomeLEDs : public Behaviour
{
	Adafruit_NeoPixel* _strip;
	uint32_t _color;

public:
	LightSomeLEDs(Adafruit_NeoPixel* strip, String name = "LightSome", uint32_t color=0xFFFFFFFF);
	char* args();
	String start(String args);
};

/** \class LightAllLEDs
    \brief Behaviour that lights up all the LEDs to a brightness specified as
		its argument.
*/
class LightAllLEDs : public Behaviour
{
	Adafruit_NeoPixel* _strip;
	uint32_t _hue;
	uint32_t _sat;

public:
	LightAllLEDs(Adafruit_NeoPixel* strip, String name = "LightAll", uint32_t hue=0, uint32_t sat=0);
	char* args();
	String start(String args);

};

/** \class BreatheLEDs
    \brief Behaviour that cycles all LEDs from full to minimum brightness and
		back at a speed specified as its argument.
*/
class BreatheLEDs : public Behaviour
{
	Adafruit_NeoPixel* _strip;
	uint _hue;
	uint _sat;
	int32_t _current = 0;
	 //Allows us to have slightly slower behaviours on the go...
	int _factor = 4;
	int _rate = 0;
	int _direction = 1;

public:
	BreatheLEDs(Adafruit_NeoPixel* strip, String name = "Breathe", uint32_t hue=0, uint32_t sat=0);
	char* args();
	String start(String args);
	void update();

};

/** \class LightHappyOrSad
    \homemade fn that linear-fades between yellow (happy) and blue (sad) - intended for face-tracker but can take any val between 0 & 255.
*/
class LightHappyOrSad : public Behaviour
{
	Adafruit_NeoPixel* _strip;
	uint32_t _color;

public:
	LightHappyOrSad(Adafruit_NeoPixel* strip, String name = "sadness");
	char* args();
	String start(String args);
};


/** \class Col
    \homemade fn that sets a colour based on (r,g,b) vals
*/
class Col : public Behaviour
{
	Adafruit_NeoPixel* _strip;
	uint32_t _color;

public:
	Col(Adafruit_NeoPixel* strip, String name = "col");
	char* args();
	String start(String args);
};




/** \class PulseCol
    \homemade adaptation of BreatheLEDs that doesn't rely on a preset colour
*/
class PulseCol : public Behaviour
{
	Adafruit_NeoPixel* _strip;
	int32_t _current = 0;
	 //Allows us to have slightly slower behaviours on the go...
	int _factor = 4;
	int _rate = 0;
	int _direction = 1;
	int val, col_1, col_2, col_3;

public:
	PulseCol(Adafruit_NeoPixel* strip, String name = "PulseCol");
	char* args();
	String start(String args);
	void update();

};


/** \class BlinkCol
    \homemade adaptation of BreatheLEDs that doesn't rely on a preset colour
*/
class BlinkCol : public Behaviour
{
	Adafruit_NeoPixel* _strip;
	int32_t _current = 0;
	 //Allows us to have slightly slower behaviours on the go...
	int _factor = 4;
	int _rate = 0;
	int _direction = 1;
	int val, col_1, col_2, col_3;

public:
	BlinkCol(Adafruit_NeoPixel* strip, String name = "BlinkCol");
	char* args();
	String start(String args);
	void update();

};


#endif
