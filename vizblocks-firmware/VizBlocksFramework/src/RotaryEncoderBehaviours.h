#ifndef ROTARYENCODER_BEHAVIOUR_h
#define ROTARYENCODER_BEHAVIOUR_h
#include <Arduino.h>
#include "Behaviours.h"
#include "RotaryEncoder.h"
#include <VizBlocks.h>

/** \file RotaryEncoderBehaviours.h
    \brief Behaviours to communicate rotary encoder events to a server via mqtt.
		A lot of these input behaviours are quite repeative and could be replaced by
		a master function with some extra parameters, but I liked the specificity
		here for legibility.
*/

/** \class RotaryEncoderUpdated
    \brief Behaviour that publishes a
 	  "RotaryEncoderUpdated" message to the block's announcment topic of the
 	  MQQT broker.
*/
class RotaryEncoderUpdated : public Behaviour
{
	VizBlock* _node;

public:
	RotaryEncoderUpdated(VizBlock* node, String name = "RotaryEncoderUpdated");
	char* args();
	String start(String args);
};

#endif
