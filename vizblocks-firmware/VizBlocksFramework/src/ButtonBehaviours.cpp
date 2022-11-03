#include "ButtonBehaviours.h"

ButtonHeld::ButtonHeld(VizBlock* node, String name) : Behaviour(name), _node(node)
{ }

String ButtonHeld::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Input\":{\"type\":\"" + String(name()) + "\",\"button\":\"" + args + "\"}}";
	_node->announce(str);

	return "ButtonHeld behaviour " + _name + " with (" + args + ")";
};

char * ButtonHeld::args()
{
	return "<String buttonId>";
};
ButtonReleased::ButtonReleased(VizBlock* node, String name) :
	Behaviour(name), _node(node)
{}

String ButtonReleased::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Input\":{\"type\":\"" + String(name()) + "\",\"button\":\"" + args + "\"}}";
	_node->announce(str);

	return "ButtonReleased behaviour " + _name + " with (" + args + ")";
};

char * ButtonReleased::args()
{
	return "<String buttonId>";
};

ButtonClicked::ButtonClicked(VizBlock* node, String name) :
	Behaviour(name), _node(node)
{ }

String ButtonClicked::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Input\":{\"type\":\"" + String(name()) + "\",\"button\":\"" + args + "\"}}";
	_node->announce(str);

	return "ButtonClicked behaviour " + _name + " with (" + args + ")";
};

char * ButtonClicked::args()
{
	return "<String buttonId>";
};

ButtonPressed::ButtonPressed(VizBlock* node, String name) :
	Behaviour(name), _node(node)
{}

String ButtonPressed::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Input\":{\"type\":\"" + String(name()) + "\",\"button\":\"" + args + "\"}}";
	_node->announce(str);

	return "ButtonPressed behaviour " + _name + " with (" + args + ")";
};

char * ButtonPressed::args()
{
	return "<String buttonId>";
};

ButtonTick::ButtonTick(VizBlock* node, String name) :
  Behaviour(name), _node(node)
{ }

String ButtonTick::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Input\":{\"type\":\"" + String(name()) + "\",\"button\":\"" + args + "\"}}";
	_node->announce(str);

	return "ButtonTick behaviour " + _name + " with (" + args + ")";
};

char * ButtonTick::args()
{
	return "<String buttonId>";
};
