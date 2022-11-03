#include "Button.h"

Button::Button(int pin, int id) : _pin(pin), _id(id) {
	pinMode(_pin, INPUT_PULLUP);
	_state = _read();
	_previousTimer = millis();
}

void Button::check() {
	unsigned long timer = millis();
	unsigned long deltaTime = timer - _previousTimer;
	_state = _read();

	if (_inputFlag == true) {
		_inputFlag = false;

		// Button pressed
		if (_state == LOW
		    && _pressedFlag == false) {
			_pressedFlag = true;
			_previousTimer = timer;
			_cb(this, kEventPressed, _state);
			return;

			// Button clicked
		} else if (_state == HIGH
		           && deltaTime < _clickInterval
		           && _holdFlag == false) {
			_pressedFlag = false;
			_previousTimer = timer;
			_cb(this, kEventClicked, _state);
			return;

			// Button released
		} else if (_state == HIGH) {
			_pressedFlag = false;
			_holdFlag = false;
			_previousTimer = timer;
			_cb(this, kEventReleased, _state);
			return;
		}
	}

	// Button held
	if (_state == LOW
	    && deltaTime > _holdInterval
	    && _holdFlag == false) {
		_holdFlag = true;
		_previousTimer = timer;
		_cb(this, kEventHeld, _state);
		return;

		// Button tick
	} else if (_state == LOW
	           && deltaTime > _repeatInterval
	           && _holdFlag == true) {
		_previousTimer = timer;
		_cb(this, kEventTick, _state);
		return;
	}
};

int Button::getHoldInterval() {
	return _holdInterval;
};

void Button::_setHoldInterval(int x) {
	_holdInterval = x;
};

int Button::getId() {
	return _id;
};

int Button::getRepeatInterval() {
	return _repeatInterval;
};

void Button::setEventHandler(void (*function)(Button*, uint8_t, bool)) {
	_cb = function;
};

void Button::_setRepeatInterval(int x) {
	_repeatInterval = x;
};

bool Button::getState() {
	return _state;
};

bool Button::_read() {
	return digitalRead(_pin);
};

int Button::getClickInterval() {
	return _clickInterval;
};

void Button::_setClickInterval(int x) {
	_clickInterval = x;
};

void Button::initInterrupts(void (*function)()) {
	attachInterrupt(_pin, function, CHANGE);
};

void Button::tick() {
	_inputFlag = true;
}
