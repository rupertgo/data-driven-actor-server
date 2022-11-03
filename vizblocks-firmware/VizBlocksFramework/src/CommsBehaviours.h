#ifndef COMMS_BEHAVIOUR_h
#define COMMS_BEHAVIOUR_h
#include <Arduino.h>
#include "Behaviours.h"
#include <VizBlocks.h>

/** \file CommsBehaviours.h
    \brief Behaviours that send useful information to a server or other blocks.
		These are a bit weird and experimental but quite useful when trying to use
		blocks to interact with a web app (for example).
*/

/** \class SendCapabilities
    \brief Behaviour that publishes a series of messages listing all the the
		block's abilities to the block's announcment topic of the MQQT broker.
*/
class SendCapabilities : public Behaviour
{
	VizBlock* _node;

public:
	SendCapabilities(VizBlock* node, String name = "SendCapabilities");
	String start(String args);

};

/** \class Link
    \brief Experimental Behaviour leverages the ability to deliver commands over
		serial. If a block is connected to another via TX -> RX pins, it can deliver
		this command and connected block will message the server that they have linked
		together. If the blocks unlink the behaviour will time out and an unlink
		message will be sent.
*/
class Link : public Behaviour
{
	VizBlock* _node;
	String _peerId;
	const int _timeoutInterval = 5000;
	unsigned long _t = 0;

public:
	Link(VizBlock* node, String name = "Link");
	char* args();
	String start(String args);
	void update();

};

/** \class PingServer
    \brief Experimental Behaviour that will regularly send a short message to the
		server so that the server knows the block is still online.
*/
class PingServer : public Behaviour
{
	VizBlock* _node;
	String str;
	const int _interval = 4000;
	unsigned long _t = 0;

public:
	PingServer(VizBlock* node, String name = "PingServer");
	String start(String args);
	void update();

};

#endif
