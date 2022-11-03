#include "CommsBehaviours.h"

SendCapabilities::SendCapabilities(VizBlock* node, String name) :
  Behaviour(name), _node(node)
{ }

String SendCapabilities::start(String args)
{
	 //This is where you do your stuff for a simple behaviour
	_node->announce_capabilities();

	return "SendCapabilities behaviour " + _name;
};

PingServer::PingServer(VizBlock* node, String name) : Behaviour(name), _node(node)
{
	_background = true;
}

void PingServer::update()
{
	if (millis() > (_t+_interval))
	{
		_t = millis();
		_node->announce(str);
	}
};

String PingServer::start(String args)
{
	_background = true;
	_running = true;
	_t = millis();
	str = "{\"id\":\"" + String(_node->getId()) + "\",\"PingServer\":{}}";
	_node->announce(str);

	return "Pinging server";
};

Link::	Link(VizBlock* node, String name) : Behaviour(name), _node(node)
	{
		_background = true;
	}


void Link::update()
{
	if (millis() > (_t+_timeoutInterval))
	{
		String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Unlink\":{\"peerId\":\"" + _peerId + "\"}}";
		_node->announce(str);
		_peerId = "";
		_running = false;
	}
};

String Link::start(String args)
{
	_running = true;

	if (args == name() || args.indexOf(" ")>0)
	{
		return "Invalid args (" + args + ") in behaviour " + name();
	}

	_t = millis();

	if (args == _peerId)
	{
		return "Link ping from (" + _peerId + ")";
	}

	_peerId = args;

	String str = "{\"id\":\"" + String(_node->getId()) + "\",\"Link\":{\"peerId\":\"" + _peerId + "\"}}";
	_node->announce(str);

	return "New link with (" + _peerId + ")";
};

char * Link::args()
{
	return "<String peerId>";
};
