#include "LEDBehaviours.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector; using std::istringstream;
using std::stringstream;

//---

LightSomeLEDs::LightSomeLEDs(Adafruit_NeoPixel* strip, String name, uint32_t color) :
	Behaviour(name), _strip(strip), _color(color)
{ }

String LightSomeLEDs::start(String args)
{
	int val = args.toInt();
	 //Always clear the strip first
	_strip->clear();

	if( val > 0 )
	{
		_strip->fill(_color, 0, val);
	}

	_strip->show();

	return "";
};

char * LightSomeLEDs::args()
{
	return "<int num_leds>";
};

//---

LightAllLEDs::LightAllLEDs(Adafruit_NeoPixel* strip, String name, uint32_t hue, uint32_t sat) :
	Behaviour(name), _strip(strip), _hue(hue), _sat(sat)
{ }

String LightAllLEDs::start(String args)
{
	int val = args.toInt();
	_strip->clear();
	_strip->fill(_strip->ColorHSV(_hue,_sat,val));
	_strip->show();

	return "";
};

char * LightAllLEDs::args()
{
	return "<int brightness>";
};

//---

BreatheLEDs::BreatheLEDs(Adafruit_NeoPixel* strip, String name, uint32_t hue, uint32_t sat) :
	Behaviour(name), _strip(strip), _hue(hue * 255), _sat(sat)
{ }

void BreatheLEDs::update()
{
	if( _rate <= 0 )
	{
		_strip->fill(0);
		_strip->show();

		return;
	}

	_current = _current + (_rate * _direction);

	if( _current < 0 )
	{
		_current = 0;
		_direction = 1;
	}

	if( _current > 255 * _factor )
	{
		_current = 255 * _factor;
		_direction = -1;
	}

	_strip->fill(_strip->ColorHSV(_hue,_sat,_current / _factor));
	_strip->show();

};

String BreatheLEDs::start(String args)
{
	_current = 0;
	_direction = 1;
	_running = true;
	int val = args.toInt();
	_rate = val;

	return "";
};

char * BreatheLEDs::args()
{
	return "<int rate (1-255ish)>";
};

//---
LightHappyOrSad::LightHappyOrSad(Adafruit_NeoPixel* strip, String name) :
	Behaviour(name), _strip(strip)
{ }

String LightHappyOrSad::start(String args)
{
	int val = args.toInt();
	 //Always clear the strip first
	_strip->clear();

	if( val > 0 )
	{
		_strip->fill(((255-val,255-val,val)), 0, 12);
	}

	_strip->show();

	return "";
};

char * LightHappyOrSad::args()
{
	return "<int num_leds>";
};



//---
Col::Col(Adafruit_NeoPixel* strip, String name) :
	Behaviour(name), _strip(strip)
{ }

String Col::start(String args)
{
	//int val = args.toInt();

    int val_1, val_2, val_3;
	const char* argslist = args.c_str();
	sscanf(argslist,"%i %*[,] %i %*[,] %i",&val_1,&val_2,&val_3);

	Serial.println(val_1);
	Serial.println(val_2);
	Serial.println(val_3);


	 //Always clear the strip first
	_strip->clear();

	_strip->fill((_strip->Color(val_1,val_2,val_3)), 0, 12);

	_strip->show();

	return "";
};

char * Col::args()
{
	return "<int num_leds>";
};








//---

PulseCol::PulseCol(Adafruit_NeoPixel* strip, String name) :
	Behaviour(name), _strip(strip)
{ }



void PulseCol::update()
{
	if( _rate <= 0 )
	{
		_strip->fill(0);
		_strip->show();

		return;
	}

	_current = _current + (_rate * _direction);

	if( _current < 0 )
	{
		_current = 0;
		_direction = 1;
	}

	if( _current > 255 * _factor )
	{
		_current = 255 * _factor;
		_direction = -1;
	}

	float brightness = _current / _factor;
	brightness = brightness/255;

	float r_brightness = brightness * col_1;
	float g_brightness = brightness * col_2;
	float b_brightness = brightness * col_3;

	_strip->fill((_strip->Color(col_1-r_brightness,col_2-g_brightness,col_3-b_brightness)), 0, 12);
	_strip->show();


};

String PulseCol::start(String args)
{
	
	const char* argslist = args.c_str();
	sscanf(argslist,"%i %*[,] %i %*[,] %i %*[,] %i",&val,&col_1,&col_2,&col_3);

	_current = 0;
	_direction = 1;
	_running = true;
	_rate = val;

	return "";
};

char * PulseCol::args()
{
	return "<int rate (1-255ish)>";
};


//---

BlinkCol::BlinkCol(Adafruit_NeoPixel* strip, String name) :
	Behaviour(name), _strip(strip)
{ }

void BlinkCol::update()
{
	if( _rate <= 0 )
	{
		_strip->fill(0);
		_strip->show();

		return;
	}

	_current = _current + (_rate * _direction);

	if( _current < 0 )
	{
		_current = 0;
		_direction = 1;
	}

	if( _current > 255 * _factor )
	{
		_current = 255 * _factor;
		_direction = -1;
	}


	if ((_current/_factor) > 220){
		_strip->fill(_strip->Color(0,0,0), 0, 6);
		Serial.println("BLINK");
	}
	else{
		_strip->fill(_strip->Color(col_1,col_2,col_3), 0, 12);
		Serial.println("NOT BLINK");
    }
	
	_strip->show();



};

String BlinkCol::start(String args)
{
	const char* argslist = args.c_str();
	sscanf(argslist,"%i %*[,] %i %*[,] %i %*[,] %i",&val,&col_1,&col_2,&col_3);

	_current = 0;
	_direction = 1;
	_running = true;
	_rate = val;
	
	_strip->clear();

	return "";
};

char * BlinkCol::args()
{
	return "<int rate (1-255ish)>";
};

