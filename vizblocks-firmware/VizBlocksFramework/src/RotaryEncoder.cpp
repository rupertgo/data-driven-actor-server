#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int id) : _pinA(pinA), _pinB(pinB), _id(id)
{
	pinMode(_pinA, INPUT_PULLUP);
	pinMode(_pinB, INPUT_PULLUP);

	_previousTimer = millis();
	_setState(digitalRead(_pinA), digitalRead(_pinB));
	setPosition(0);
}

void RotaryEncoder::check()
{
	unsigned long timer = millis();
	unsigned long deltaTime = timer - _previousTimer;

	if (_inputFlag == true)
	{
		_inputFlag = false;

		_changeFlag = true;
		_previousTimer = timer;

		_cb(this, kEventUnstableUpdate, getPostition());
	}

	if (_changeFlag == true && deltaTime > _interval)
	{
		_changeFlag = false;

		_cb(this, kEventStableUpdate, getPostition());
	}
};

int RotaryEncoder::getId()
{
	return _id;
};

void RotaryEncoder::_setState(int a, int b)
{
	_state[0] = a;
	_state[1] = b;
};

void RotaryEncoder::setPosition(int value)
{
	_position = value;
};

void RotaryEncoder::_incrementPosition(int delta)
{
	_position = _position + delta;
};

boolean RotaryEncoder::_compareArrays(int a[4], int b[4])
{
	if (a[0] != b[0])
	{
		return false;
	}

	if (a[1] != b[1])
	{
		return false;
	}

	if (a[2] != b[2])
	{
		return false;
	}

	if (a[3] != b[3])
	{
		return false;
	}

	return true;
};

void RotaryEncoder::initInterrupts(void (*function)())
{
	attachInterrupt(_pinA, function, CHANGE);
	attachInterrupt(_pinB, function, CHANGE);
};

int RotaryEncoder::getPostition()
{
	return _position;
};

int RotaryEncoder::_findChange(int state1[2], volatile int state2[2])
{
	int stateAppend[] = {state1[1], state1[0], state2[1], state2[0]};

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (_compareArrays(stateAppend, movements[i][j]))
			{
				if (i == 0)
				{
					return 0;
				}
				else
				if (i == 1)
				{
					return 1;
				}
				else
				if (i == 2)
				{
					return -1;
				}
				else
				if (i == 3)
				{
					return 2;
				}
				else
				if (i == 4)
				{
					return -2;
				}
			}
		}
	}

	for (int i = 3; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (_compareArrays(stateAppend, movements[i][j]))
			{
				if (i == 3)
				{
					return 2;
				}
				else
				if (i == 4)
				{
					return -2;
				}
			}
		}
	}

	Serial.println("INVALID DATA");

	return 0;
};

void RotaryEncoder::setEventHandler(void (*function)(RotaryEncoder*, uint8_t, int))
{
	_cb = function;
};
void RotaryEncoder::tick() {
	_inputFlag = true;
}
