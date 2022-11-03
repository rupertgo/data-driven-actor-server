#ifndef POTENTIOMETER_BEHAVIOUR_h
#define POTENTIOMETER_BEHAVIOUR_h
#include <Arduino.h>
#include "Behaviours.h"
#include "Potentiometer.h"
#include <VizBlocks.h>

/** \file PotentiometerBehaviours.h
    \brief Behaviours to communicate potentiometer events to a server via mqtt.
		A lot of these input behaviours are quite repeative and could be replaced by
		a master function with some extra parameters, but I liked the specificity
		here for legibility.
*/

/** \class PotentiometerUpdated
    \brief Behaviour that publishes a
 	  "PotentiometerUpdated" message to the block's announcment topic of the
 	  MQQT broker.
*/
class PotentiometerUpdated : public Behaviour
{
	VizBlock* _node;

public:
	PotentiometerUpdated(VizBlock* node, String name = "PotentiometerUpdated");
	char* args();
	String start(String args);
};

#endif
