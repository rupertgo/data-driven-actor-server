#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin, int id) : _pin(pin), _id(id)
{
	pinMode(pin, INPUT);
	EMA_S = analogRead(_pin);  //set EMA S for t=1
	_value = EMA_S;
	_previousReading = EMA_S;

	_previousTimer = millis();
	_inputFlag = true;
};

void Potentiometer::check()
{
	unsigned long timer = millis();
	unsigned long deltaTime = timer - _previousTimer;

	int reading = _read();
	int deltaValue = abs(reading - _value);

	if (reading != _value)
	{
		_inputFlag = true;
	}

	if (_inputFlag == true && deltaValue > 1)
	{
		_inputFlag = false;
		_changeFlag = true;
		_previousTimer = timer;
		_setValue(reading);
		_cb(this, kEventUnstableUpdate, getValue());
	}

	if (_changeFlag == true &&  deltaTime > _interval)
	{
		_changeFlag = false;
		_cb(this, kEventStableUpdate, getValue());
	}
};

void Potentiometer::_setValue(int x)
{
	_value = x;
};

int Potentiometer::getValue()
{
	return _value;
};

int Potentiometer::_read()
{
	int sensorValue = analogRead(_pin);                //read the sensor value using ADC
	EMA_S = (EMA_a*sensorValue) + ((1-EMA_a)*EMA_S);   //run the EMA
	int mappedValue = map(EMA_S, 5, 1023, 0, 100);

	return mappedValue;
};

int Potentiometer::getId()
{
	return _id;
};

void Potentiometer::setEventHandler(void (*function)(Potentiometer*, uint8_t, uint8_t))
{
	_cb = function;
};
