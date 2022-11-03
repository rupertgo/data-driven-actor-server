#ifndef BUTTON_h
#define BUTTON_h

#include <Arduino.h>

/** \file Button.h
    \brief Event-driven button functionality.
    This file contains a class for an event-driven button input. It is
		based on the [AceButton Library by bxparks](https://github.com/bxparks/AceButton).
		The implimentation here is configured for esp822 boards and simplified. It
		is however, far less configurable.
*/

/** \class Button
    \brief Event-driven button class.
*/
class Button
{

private:

	int _pin;
	int _id;

	bool _state;

	volatile bool _inputFlag = false;
	bool _changeFlag = false;

	bool _pressedFlag = false;
	bool _holdFlag = false;

	unsigned long _previousTimer;

	int _clickInterval = 1000;
	int _holdInterval = 1000;
	int _repeatInterval = 1000;

	void (*_cb)(Button*, uint8_t, bool);

	/**
	   @brief Reads the button pin and returns the value.
	 */
	bool _read();

	/**
	   @brief Configures the timing for kEventClicked to be detected.
	 */
	void _setClickInterval(int x);

	/**
	   @brief Configures the timing for kEventHeld to be detected.
	 */
	void _setHoldInterval(int x);

	/**
	   @brief Configures the timing for kEventTick to be detected.
	 */
	void _setRepeatInterval(int x);

public:

	/**
	   @brief Event ID for button press.
	 */
	static const uint8_t kEventPressed = 0;

	/**
		 @brief Event ID for button release.
	 */
	static const uint8_t kEventReleased = 1;

	/**
		 @brief Event ID for button click (pressed and released within `_clickInterval`).
	 */
	static const uint8_t kEventClicked = 2;

	/**
		 @brief Event ID for button hold (held down for longer than `_holdInterval`).
	 */
	static const uint8_t kEventHeld = 3;

	/**
		 @brief Event ID for button tick (released every `_repeatInterval` when button held).
	 */
	static const uint8_t kEventTick = 4;

	/**
		 @brief Class constructor.
	 */
	Button(int pin, int id = 99);

	/**
	   @brief Set the function to be called when a pin interrupt occurs.
		 Ideally this wouldn't be needed and the pin interrupt would just call
		 Button::tick(), but only static member functions can be used as callbacks
		 for pin interrupts, so a wrapper function has to be written in the global
		 scope and passed back into the button object.
	 */
	void initInterrupts(void (*function)());

	/**
	   @brief Set the function that is called any time a button event is detected.
		 Some example callback functions can be found in the InputNode example.
	 */
	void setEventHandler(void (*function)(Button*, uint8_t, bool));

	/**
	   @brief Returns the current button state.
	 */
	bool getState();

	/**
	   @brief Returns the button ID.
	 */
	int getId();

	/**
	   @brief Returns button's click interval.
	 */
	int getClickInterval();

	/**
	   @brief Returns button's hold interval.
	 */
	int getHoldInterval();

	/**
	   @brief Returns button's tick interval.
	 */
	int getRepeatInterval();

	/**
	   @brief Polls the button's _inputFlag and detects events.
		 This should be called in the sketch's loop() function.
	 */
	void check();

  /**
      @brief Raises the button's _inputFlag.
			This should be wrapped in a function in the global scope and fed into
			Button::initInterrupts()
  */
  void 	ICACHE_RAM_ATTR tick();

};

#endif
