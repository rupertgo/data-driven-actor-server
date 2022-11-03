#ifndef POTENTIOMETER_h
#define POTENTIOMETER_h

#include <Arduino.h>

/** \file Potentiometer.h
    \brief Slider and rotary potentiometer functionality.

    This file contains a class for a psuedo-event-driven potentiometer input.
		It is inspired by the event-driven logic of the Button class, but analogue
		pins on the esp8266 cannot have interrupts attached to them so it polls
		the value of the pin, rather than a change flag. The readings are filtered
		to improve stability following
		[this norwegian creations tutorial](https://www.norwegiancreations.com/2015/10/tutorial-potentiometers-with-arduino-and-filtering/).
*/

/** \class Potentiometer
    \brief Psuedo-event-driven potentiometer class.
		Events are split into stable and unstable updates to prevent flooding the
		mqtt channel with many messages while the slider is being set, while allowing
		for a local display element (i.e. an led) to give real-time feedback.
*/
class Potentiometer
{

	int EMA_S = 0;
	float EMA_a = 0.6;

	int _pin;
	int _id;

	int _value;
	int _previousReading;

	bool _inputFlag = false;
	bool _changeFlag = false;

	unsigned long _previousTimer;
	int _interval = 200;

	void (*_cb)(Potentiometer*, uint8_t, uint8_t);

	/**
	   @brief Reads the button pin and returns the value.
	 */
	int _read();

	/**
	   @brief Artificially set the potentiometer value.
	 */
	void _setValue(int x);

public:

	/**
	   @brief Event ID for a stable update (potentiometer has been changed, then been stable longer than `_interval`).
	 */
	static const uint8_t kEventStableUpdate = 0;

	/**
		 @brief Event ID for a stable update (potentiometer has been changed, but not stopped moving yet).
	 */
	static const uint8_t kEventUnstableUpdate = 1;

	/**
	   @brief Class constructor.
	 */

	Potentiometer(int pin, int id = 99);

	/**
	   @brief Set the function that is called any time a button event is detected.
		 Some example callback functions can be found in the InputNode example.
	 */
	void setEventHandler(void (*function)(Potentiometer*, uint8_t, uint8_t));

	/**
	   @brief Returns the current stable potentiometer value.
	 */
	int getValue();

	/**
	   @brief Returns the potentiometer ID.
	 */
	int getId();

	/**
	   @brief Polls the potentiometer's value and detects events.
		 This should be called in the sketch's loop() function.
	 */
	void check();
};

#endif
