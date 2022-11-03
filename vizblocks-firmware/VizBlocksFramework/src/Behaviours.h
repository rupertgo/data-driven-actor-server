#ifndef BEHAVIOUR_h
#define BEHAVIOUR_h
#include <Arduino.h>

/** \file Behaviours.h
    \brief Functionality for block behaviours.

    This file contains the parent Behaviour class and a BehaviourTable class for
		storing the array of a block's capabilities.
*/

/** \class Behaviour
    \brief Parent class for all other behaviours.

    This class contains is a template for building block behaviours that can
		do just about anything you could dream up. It has a bunch of virtual methods
		that you can override in its children.
*/
class Behaviour
{
protected:
	boolean _interruptable = true;
	boolean _temp = false;
	boolean _priority = false;
	boolean _running = false;
	boolean _background = false;
	String _name = "name";

public:
	Behaviour(String name);
	~Behaviour();

	/**
	   Can this behaviour be interruped?
	 */
	virtual boolean is_interruptable();

	/**
	   Can this behaviour be run quickly without stopping what's going on (e.g. comms, debug)?
	 */
	virtual boolean is_temp();

	/**
	   Should this behaviour override others?
	 */
	virtual boolean is_priority();

	/**
	   Is the behaviour running?
	 */
	virtual boolean is_running();

	/**
	   Can the behaviour run in the background?
	 */
	virtual boolean is_background();

	/**
	   What's the name of this behaviour?
	 */
	virtual String name();

	/**
	   What arguments does the behaviour take?
		 Override this to document your behaviour.
	 */
	virtual char* args();
	/**
	   Start the behaviour, with arguments.
	 */
	virtual String start(String args);
	/**
	   Update the behaviour periodically.
	 */
	virtual void update();
	/**
	   Stop the behaviour. This can be useful to stop the behaviour after it has
		 updated a set number of times, or after a fixed amount of time.
	 */
	virtual void stop();
};

/** \class BehaviourTable
    \brief A container for a block's behaviours.

    This class allows for up to 40 behaviours to be stored and retrieved.
*/
class BehaviourTable
{
	Behaviour* behaviours[40];

public:
	int num = 0;

	BehaviourTable();

	/**
	   Add a behaviour to the table.
	 */
	void add(Behaviour *b);

	/**
		 Search for and return a behaviour by name.
	 */
	Behaviour* get(String n);

	/**
		 Search for and return a behaviour by index.
	 */
	Behaviour* get_by_num(int n);

	/**
		 Return the number of behaviours in the table.
	 */
	int get_num_behaviours();
};

#endif
