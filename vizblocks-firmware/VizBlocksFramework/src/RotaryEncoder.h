#ifndef ROTARYENCODER_h
#define ROTARYENCODER_h

#include <Arduino.h>

/** \file RotaryEncoder.h
    \brief Event-driven rotary encoder functionality.

    This file contains a class for a event-driven RotaryEncoder input.
		It is inspired by the event-driven logic of the Button class.
*/

/** \class RotaryEncoder
    \brief Event-driven rotary encoder class.
		Events are split into stable and unstable updates to prevent flooding the
		mqtt channel with many messages while the slider is being set, while allowing
		for a local display element (i.e. an led) to give real-time feedback.
*/
class RotaryEncoder
{
	int _pinA;
	int _pinB;
	int _id;

	volatile int _state[2];
	volatile int _position;

	volatile bool _inputFlag = false;
	bool _changeFlag = false;

	unsigned long _previousTimer;
	int _interval = 200;

	void (*_cb)(RotaryEncoder*, uint8_t, int);

	/**
	   @brief Potential pin states and the movement they show
		 | newPin2 | newPin1 | oldPin2 | oldPin1 |
	 */
	int movements[5][4][4] = {
		{ // No movement
			{0, 0, 0, 0},
			{0, 1, 0, 1},
			{1, 0, 1, 0},
			{1, 1, 1, 1}
		},
		{ // +1
			{0, 0, 0, 1},
			{0, 1, 1, 1},
			{1, 0, 0, 0},
			{1, 1, 1, 0}
		},
		{ // -1
			{0, 0, 1, 0},
			{0, 1, 0, 0},
			{1, 0, 1, 1},
			{1, 1, 0, 1}
		},
		{ // +2
			{0, 0, 1, 1},
			{1, 1, 0, 0}
		},
		{ // -2
			{0, 1, 1, 0},
			{1, 0, 0, 1}
		},
	};

	/**
	   @brief Artificially set the encoder state.
	 */
	void _setState(int a, int b);

	/**
	   @brief Artificially change the encoder position.
	 */
	void _incrementPosition(int delta);

	/**
	   @brief Compare new and old encoder state to see if a change has occured.
	 */
	int _findChange(int state1[2], volatile int state2[2]);

	/**
	   @brief Compare the encoder state with an array of potential options to detect direction of motion.
	 */
	boolean _compareArrays(int a[4], int b[4]);

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
	RotaryEncoder(int pinA, int pinB, int id = 99);

	/**
	   @brief Set the function to be called when a pin interrupt occurs.
		 Ideally this wouldn't be needed and the pin interrupt would just call
		 RotaryEncoder::tick(), but only static member functions can be used as
		 callbacks for pin interrupts, so a wrapper function has to be written in
		 the global scope and passed back into the button object.
	 */
	void initInterrupts(void (*function)());

	/**
	   @brief Set the function that is called any time an encoder event is detected.
		 Some example callback functions can be found in the InputNode example.
	 */
	void setEventHandler(void (*function)(RotaryEncoder*, uint8_t, int));

	/**
	   @brief Returns the current encoder postion.
	 */
	int getPostition();

	/**
	   @brief Returns the encoder id.
	 */
	int getId();

	/**
	   @brief Sets the encoder postion. Useful for resetting it back to zero for example.
	 */
	void setPosition(int value);

	/**
	   @brief Polls the encoders's _inputFlag and detects events.
		 This should be called in the sketch's loop() function.
	 */
	void check();

	/**
      @brief Raises the button's _inputFlag.
			This should be wrapped in a function in the global scope and fed into
			RotaryEncoder::initInterrupts()
  */
	void ICACHE_RAM_ATTR tick();

};

#endif
