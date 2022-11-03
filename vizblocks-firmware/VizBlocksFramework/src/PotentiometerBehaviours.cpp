#include "PotentiometerBehaviours.h"

PotentiometerUpdated::PotentiometerUpdated(VizBlock* node, String name) :
	Behaviour(name), _node(node)
{ }

String PotentiometerUpdated::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	int index = args.indexOf(" ");
	String pot = "";
	String value = "";

	if ( index > 0 )
	{
		pot = args.substring(0, index);
		value = args.substring(index+1);
	}
	else
	{
		return "PotentiometerUpdated behaviour args error!";
	}

	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Input\":{\"type\":\"" + String(name()) + "\",\"pot\":\"" + pot + "\",\"value\":\"" + value + "\"}}";
	_node->announce(str);

	return "PotentiometerUpdated behaviour " + _name + " with (" + args + ")";
};

char * PotentiometerUpdated::args()
{
	return "<String potentiometerId> <int value>";
};
