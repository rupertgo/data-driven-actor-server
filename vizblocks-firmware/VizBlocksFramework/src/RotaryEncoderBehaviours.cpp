#include "RotaryEncoderBehaviours.h"

RotaryEncoderUpdated::RotaryEncoderUpdated(VizBlock* node, String name) :
	Behaviour(name), _node(node)
{ }

String RotaryEncoderUpdated::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	int index = args.indexOf(" ");
	String encoder = "";
	String position = "";

	if ( index > 0 )
	{
		encoder = args.substring(0, index);
		position = args.substring(index+1);
	}
	else
	{
		return "RotaryEncoderUpdated behaviour args error!";
	}

	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Input\":{\"type\":\"" + String(name()) + "\",\"encoder\":\"" + encoder + "\",\"position\":\"" + position + "\"}}";
	_node->announce(str);

	return "RotaryEncoderUpdated behaviour " + _name + " with (" + args + ")";
};

char * RotaryEncoderUpdated::args()
{
	return "<String rotaryEncoderId> <int position>";
};
