#include "VizBlocks.h"

VizBlock::VizBlock(char* id, char* ssid, char* wifi_pass,
          char* server, int port, char* mqtt_user, char* mqtt_pass, const char* fingerprint) : 
		  _id(id), _server(server), _port(port), _ssid(ssid), _wifi_pass(wifi_pass), _mqtt_user(mqtt_user), _mqtt_pass(mqtt_pass), _fingerprint(fingerprint)
{};

void VizBlock::add(Behaviour *b)
{
	_behaviours.add(b);
};

void VizBlock::serial_command()
{
	if( Serial.available() )
	{
		String cmd = Serial.readStringUntil('\n');
		cmd.replace("\r", "");
		Serial.println(process(cmd));
	}
};

void VizBlock::generateCapabilitiesJSON()
{
	String head = "{\"id\":\"" + String(_id) + "\",\"Behaviour\":{";

	for (int i = 0; i < _behaviours.get_num_behaviours(); i++)
	{
		Behaviour* b = _behaviours.get_by_num(i);
		String args = b->args();
		String body = "\"name\":\"" + b->name() + "\",\"args\":\"" + args + "\"}}";
		String str = head + body;
		capabilitiesJSON[i] = str;
	}
};

char * VizBlock::getId()
{
	return _id;
};

void VizBlock::MQTT_connect()
{
	int8_t ret;

	 // Stop if already connected.
	if (_mqtt->connected())
	{
		return;
	}

	Serial.println("Connecting to MQTT... ");
	
	uint8_t retries = 3;

	while ((ret = _mqtt->connect()) != 0)  // connect will return 0 for connected
	{
		Serial.println(_mqtt->connectErrorString(ret));
		Serial.println("Retrying MQTT connection in 5 seconds...");
		_mqtt->disconnect();
		delay(5000);  // wait 5 seconds
		retries--;

		if (retries == 0)
		{
			 // basically die and wait for WDT to reset me
			while (1)
				;
		}
	}

	Serial.println("MQTT Connected!");
};

void VizBlock::wifi_connect()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		return;
	}

		// Connect to WiFi access point.
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(_ssid);

	WiFi.begin(_ssid, _wifi_pass);
	_wifi_timeout_initialMillis = millis();
	_wifi_timeout = 300000; // Wait 5 minutes for wifi
	Serial.println("Checking for WiFi...");
	digitalWrite(D4, LOW);

	while (WiFi.status() != WL_CONNECTED)
	{
		unsigned long currentMillis = millis();

		if (currentMillis - _wifi_timeout_initialMillis < _wifi_timeout) 
		{
			Serial.print(".");
			delay(500);
		}
		else 
		{
			Serial.println("No WiFi found. Going to sleep. Bye!");
			ESP.deepSleep(0); // deep sleep until reset. Could use D0 to wake after a specified interval, but D0 is already in use.
		}
		

	}

	Serial.println();

	Serial.println("WiFi connected");
	Serial.println("IP address: "); 
	Serial.print(WiFi.localIP()); // Done Wifi
};

void onStationModeDisconnectedEvent(const WiFiEventStationModeDisconnected& evt) {
	if (WiFi.status() == WL_CONNECTED) {
		WiFi.disconnect();
	} 
	Serial.println("WiFi disconnected...");
	// set the LED to be on, rather than flashing
	digitalWrite(D4, LOW);
}

void VizBlock::run()
{
	int loop_start_time = millis();
	serial_command();

	if( _wifi )
	{	
		wifi_connect();
		mqtt_command();
		unsigned long current_flashMillis = millis();

		if (current_flashMillis - _previous_flashMillis >= _flash_interval) {
			// save the last time you blinked the LED
			_previous_flashMillis = current_flashMillis;

			// if the LED is off turn it on and vice-versa:
			if (_ledState == LOW) {
				_ledState = HIGH;
			} else {
				_ledState = LOW;
			}

			// set the LED with the ledState of the variable:
			digitalWrite(D4, _ledState);
		}
	}

	if( _active )
	{
		_active->update();

		if( !_active->is_running() )
		{
			_active = nullptr;
		}
	}

	for (int i = 0; i < NUM_BACKGROUND_BEHAVIOURS; i++)
	{
		if( _background[i] )
		{
			_background[i]->update();

			if( !_background[i]->is_running() )
			{
				_background[i] = nullptr;
			}
		}
	}

	int loop_time_taken = millis()-loop_start_time;

	if( loop_time_taken < _loop_time )
	{
		delay( _loop_time - loop_time_taken );
	}
};

void VizBlock::init()
{
	Serial.setTimeout(100);
	Serial.println();
	Serial.println(F("VizBlock Node starting up"));
	Serial.println("Initialising " + String(_id));
	pinMode(D4, OUTPUT);
  	digitalWrite(D4, LOW);

	WiFi.mode(WIFI_STA);
	WiFi.setSleepMode(WIFI_NONE_SLEEP);

	if( _wifi )
	{
		wifi_connect();
		// Setup MQTT
		if (_port != 8883) {
			_client = new WiFiClient();
			_mqtt = new Adafruit_MQTT_Client(_client, _server, _port, _mqtt_user, _mqtt_pass);
		} else {
			Serial.println(F("Creating SECURE client for MQTT"));
			_client_secure = new WiFiClientSecure();
			_client_secure->setFingerprint(_fingerprint);
			_mqtt = new Adafruit_MQTT_Client(_client_secure, _server, _port, _mqtt_user, _mqtt_pass);		
		}
		
		_device_subscription = new Adafruit_MQTT_Subscribe(_mqtt, _id);
		_announce = new Adafruit_MQTT_Publish(_mqtt, "announce");
		_my_announce_channel = String("announce/") + String(_id);
		_my_announce = new Adafruit_MQTT_Publish(_mqtt, _my_announce_channel.c_str());

		// Setup MQTT subscription for this device
		_mqtt->subscribe(_device_subscription);
		// This *would* setup a callback, but we're not doing this right now...
		//_device_subscription->setCallback(test_callback);
		MQTT_connect();
	}

	generateCapabilitiesJSON();
	announce_capabilities();
	Serial.println("Init finished");

};

void VizBlock::announce(String doc)
{
	if( _wifi )
	{
		_my_announce->publish(doc.c_str());
	}

	Serial.print("-->");
	Serial.println(doc);
};

void VizBlock::announce_capabilities()
{
	String doc;

	if( _wifi )
	{
		if( !_announce->publish(_id) )
		{
			Serial.println("Couldn't make announcement");
		}
	}

	for( int i = 0; i < _behaviours.get_num_behaviours(); i++ )
	{
		doc = capabilitiesJSON[i];
		announce(doc);
	}
};

void VizBlock::set_wifi(boolean v)
{
	_wifi = v;
};

void VizBlock::setID(char* id)
{
	_id = id;
};

void VizBlock::mqtt_command()
{
	MQTT_connect(); //ensure connection
	Adafruit_MQTT_Subscribe *subscription;

	while ((subscription = _mqtt->readSubscription(50)))
	{
		if (subscription == _device_subscription)
		{
			Serial.print(F("Got: "));
			Serial.println((char *)_device_subscription->lastread);
			Serial.println(process((char *)_device_subscription->lastread));
		}
	}
};

String VizBlock::process_command(String command, String args)
{
	Serial.println("Processing <"+command+"> <"+args+">");
	Behaviour* b = _behaviours.get(command);

	if(b)
	{
		 // Stop whatever behaviour is running in the active slot.
		if( _active )
		{
			_active->stop();
		}

		Serial.println( "Found behaviour: <"+command+">" );

		if ( b->is_background() )
		{
			 // If this behaviour is already running in the background, refresh it and move on.
			for (int i = 0; i < NUM_BACKGROUND_BEHAVIOURS; i++)
			{
				if ( _background[i] )
				{
					if ( b->name() == _background[i]->name() )
					{
						return( b->start(args) );
					}
				}
			}

			 // Else, if there is space for another background behaviour, start it running.
			for (int i = 0; i < NUM_BACKGROUND_BEHAVIOURS; i++)
			{
				if ( !_background[i] )
				{
					_background[i] = b;

					return( b->start(args) );
				}
			}

			return "Couldn't run command: " + command + ". Background behaviours full.";
		}

		_active = b;

		return( b->start(args) );
	}
	else
	{
		return "Couldn't process command: " + command;
	}
};

String VizBlock::process(String input)
{

	if (!(input.indexOf("<") >= 0 || input.indexOf(">") >= 0 || input.indexOf("-") >= 0))
	{
		int index = input.indexOf(" ");
		String command = "";
		String args = "";

		if( index )
		{
			command = input.substring(0,index);
			args = input.substring(index+1);
		}
		else
		{
			command = input;
		}

		return process_command(command, args);
	}
};
