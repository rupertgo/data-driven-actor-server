#ifndef VizBlock_H
#define VizBlock_H
#include <Arduino.h>
#include "Behaviours.h"
#include "NameDictionary.h"
#include "ServoBehaviours.h"
#include "LEDBehaviours.h"

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/** \file VizBlocks.h
    \brief Contains everything you need to get started.

    This file includes all of the libraries and dependancies needed to
		start experimenting with the framework.
*/

/** \class VizBlock
    \brief The main VizBlock class.

    This class contains all of the connection information for the WiFi network
		and Node-RED. It checks for commands recieved via serial/mqtt and executes
		behaviours in reponse.
*/
class VizBlock
{
	char* _ssid;
	char* _wifi_pass;
	char* _id;
	char* _server;
	int _port;
	char* _mqtt_user;
	char* _mqtt_pass;
	const char* _fingerprint;

	static const int NUM_BACKGROUND_BEHAVIOURS = 5;
	BehaviourTable _behaviours;
	Behaviour* _active = nullptr;
	Behaviour* _background[NUM_BACKGROUND_BEHAVIOURS];

	int _loop_time = 5;
	unsigned long _wifi_timeout; // How long to wait for wifi until we go into deep sleep
	unsigned long _wifi_timeout_initialMillis;
	int _ledState = LOW;
	unsigned long _previous_flashMillis = 0;
	const long _flash_interval = 1000; // flash every second when wifi and MQTT are connected


	Adafruit_MQTT_Client* _mqtt;
	Adafruit_MQTT_Subscribe* _device_subscription;
	Adafruit_MQTT_Publish* _announce;
	Adafruit_MQTT_Publish* _my_announce;
	String _my_announce_channel;
	Adafruit_MQTT_Publish* _input;
	Adafruit_MQTT_Publish* _my_input;
	String _my_input_channel;
	WiFiClient* _client;
	WiFiClientSecure* _client_secure;

	boolean _wifi;

	String capabilitiesJSON[50];

public:
	VizBlock(char* id, char* ssid="VizBlockNet", char* wifi_pass="VizBlocksAP",
	          char* server="192.168.4.1", int port=1883, char* mqtt_user="", char* mqtt_pass="", const char* fingerprint="");
	/**
		 Turns the block's wifi on or off.
	 */
	void set_wifi(boolean v);

	/**
	 * Set up the block's WiFi and MQTT.
	 */
	void init();

	/**
	 * Add a behaviour to the list of possible behaviours.
	 */
	void add(Behaviour *b);

	/**
	 * This is the main loop. It should be called from within loop() - really
	 * this function is the only thing you should need to call. It will manage
	 * it's own delay, so you can call as often as possible.
	 */
	void run();

	/**
	 * Read a command from the serial input and process it
	 */
	void serial_command();

	/**
	 * Read a command from the serial input and process it. It only waits for
	 * 50ms to allow other behaviours to continue.
	 */
	void mqtt_command();

	/**
	 * Process a command. This means:
	 * - split the command name from the arguments
	 * - call process_command with the separated command and argument string
	 */
	String process(String input);

	/**
	 * Process a command and its arguments. This means:
	 * - look for a Behaviour with the right name
	 * - if found, then call that behaviour with the arguments (which are still a single string)
	 */
	String process_command(String command, String args);

	/**
	 * Function to connect and reconnect as necessary to the MQTT server.
	 */
	void MQTT_connect();

	/**
	 * Function to connect and reconnect as necessary to the WIFI.
	 */
	void wifi_connect();

	/**
	   Generates an array json formatted string containing information about
		 each of the behaviours the block can perform.
	 */
	void generateCapabilitiesJSON();

	/**
	   Publishes a message to the block's mqtt announcement channel.
	 */
	void announce(String doc);

	/**
	   Loops through the block's behaviours and announces each one over mqtt/
	 */
	void announce_capabilities();

	/**
	   Sets the ID of the block.
	 */
	void setID(char* id);

	/**
	   Returns the ID of the block.
	 */
	char* getId();

};

/*
 * These behaviours depend on VizBlock class so they must be included after
 * it has been defined.
 */
#include "CommsBehaviours.h"
#include "ButtonBehaviours.h"
#include "PotentiometerBehaviours.h"
#include "RotaryEncoderBehaviours.h"

#endif
