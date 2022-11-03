#ifndef BUTTON_BEHAVIOUR_h
#define BUTTON_BEHAVIOUR_h
#include <Arduino.h>
#include "Behaviours.h"
#include "Button.h"
#include <VizBlocks.h>

/** \file ButtonBehaviours.h
    \brief Behaviours to communicate button events to a server via mqtt.
		A lot of these input behaviours are quite repeative and could be replaced by
		a master function with some extra parameters, but I liked the specificity
		here for legibility.
*/

/** \class ButtonPressed
    \brief Behaviour that publishes a
 	  "ButtonPressed" message to the block's announcment topic of the
 	  MQQT broker.
*/
class ButtonPressed : public Behaviour
{
	VizBlock* _node;

public:
	ButtonPressed(VizBlock* node, String name = "ButtonPressed");
	char* args();
	String start(String args);
};

/** \class ButtonReleased
    \brief Behaviour that publishes a
 	  "ButtonReleased" message to the block's announcment topic of the
 	  MQQT broker.
*/
class ButtonReleased : public Behaviour
{
	VizBlock* _node;

public:
	ButtonReleased(VizBlock* node, String name = "ButtonReleased");
	char* args();
	String start(String args);
};

/** \class ButtonClicked
    \brief Behaviour that publishes a
 	  "ButtonClicked" message to the block's announcment topic of the
 	  MQQT broker.
*/
class ButtonClicked : public Behaviour
{
	VizBlock* _node;

public:
	ButtonClicked(VizBlock* node, String name = "ButtonClicked");
	char* args();
	String start(String args);
};

/** \class ButtonHeld
    \brief Behaviour that publishes a
 	  "ButtonHeld" message to the block's announcment topic of the
 	  MQQT broker.
*/
class ButtonHeld : public Behaviour
{
	VizBlock* _node;

public:
	ButtonHeld(VizBlock* node, String name = "ButtonHeld");
	char* args();
	String start(String args);
};

/** \class ButtonTick
    \brief Behaviour that publishes a
 	  "ButtonTick" message to the block's announcment topic of the
 	  MQQT broker.
*/
class ButtonTick : public Behaviour
{
	VizBlock* _node;

public:
	ButtonTick(VizBlock* node, String name = "ButtonTick");
	char* args();
	String start(String args);
};

#endif
