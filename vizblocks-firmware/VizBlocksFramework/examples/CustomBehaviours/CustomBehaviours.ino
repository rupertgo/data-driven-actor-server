#include <VizBlocks.h>

/*
 * Classes that define custom behaviours
 */
class SimpleBehaviour : public Behaviour {
public:
  SimpleBehaviour(String name) : Behaviour(name)  {}
  /*
   * This is the bit that actually does something!
   * args has all of the information that you are given in it, i.e. if
   * your command is called 'custom', and the command was "custom 50",
   * args would be "50"
   */
  String start(String args) {
    String msg =  "This behaviour executes the code in it's start() function once each time \
                  the block receives a \'" + _name + "\' command. Any arguments you add after \
                  the command will be available as a string, for example: (" + args + ")";
    return msg;
  }
};

class LoopingBehaviour : public Behaviour {
  int _interval;
  unsigned long _time;
public:
  LoopingBehaviour(String name, int interval = 5000) : Behaviour(name), _interval(interval)  {}
  /*
   * This is the bit that actually does something!
   * args has all of the information that you are given in it, i.e. if
   * your command is called 'custom', and the command was "custom 50",
   * args would be "50"
   */
  String start(String args) {
    _running = true;
    _time = millis();
    String msg =  "This behaviour also executes the code in its start() function once each \
                  time the block receives a \'" + _name + "\' command. Any arguments you add \
                  after the command will be available as a string, for example: (" + args + ").";
    return msg;
  }
  void update() {
    if (millis() > (_time+_interval)) {
      _time = millis();
      String msg =  "However, it will also run the code in its update() function every time the \
                    code loops until another command is recieved by the block. In this example we have \
                    set an interval so this message prints every " + String(_interval) + " milliseconds.";
      Serial.print(msg);
    }
  }
};

class BackgroundBehaviour : public Behaviour {
  int _delay;
  unsigned long _time;
public:
  BackgroundBehaviour(String name, int delay = 15000) : Behaviour(name), _delay(delay)  {
    _background = true;
  }
  /*
   * This is the bit that actually does something!
   * args has all of the information that you are given in it, i.e. if
   * your command is called 'custom', and the command was "custom 50",
   * args would be "50"
   */
  String start(String args) {
    _running = true;
    _time = millis();
    String msg =  "This behaviour also executes the code in its start() function once each \
                  time the block receives a \'" + _name + "\' command. Any arguments you add \
                  after the command will be available as a string, for example: (" + args + ").";
    return msg;
  }
  void update() {
    if (millis() > (_time+_delay)) {
      _running = false;
      String msg =  "However, it will also run the code in its update() function every time the \
                    code loops even when another command is recieved by the block. In this example we have \
                    set a timer so this message prints once after " + String(_delay) + " milliseconds.";
      Serial.print(msg);
    }
  }
};

/*
 * Get device name
 */
const String device_id = String(ESP.getChipId(), HEX);
NameDictionary d;
const String name = d.get(device_id);
char _name[10];

/*
 * Init as a VizBlock node with "null" ID
 */
VizBlock node(
  "null",           // Block ID
  "NodeRedServer",  //Wifi Access Point
  "NodeR3dAP",      //WiFi Password
  "192.168.4.1",    //IP address of Node RED server
  1883              //Port for Node RED server
  );

void setup()
{
  /*
   * Get the serial port ready.
   */
  Serial.begin(115200);
  Serial.println("Serial started!");
  delay(500);

  /*
   * Gives the VizBlock a unique name.
   * This will be a 3 letter name is the WEMOS's chip ID is registered in
   * ".NameDictionary.h", or the WEMOS's chip ID if it is not registered yet.
   */
  if (name == device_id) {
    Serial.println("!!! This device doesn't have a name yet. Let's call it: " + name);
  } else {
    Serial.println("Device name: " + name);
  }
  name.toCharArray(_name, 10);
  node.setID(_name);

  /*
   * Add in the custom behaviour we defined earlier.
   */
  node.add( new SimpleBehaviour("simple") );
  node.add( new LoopingBehaviour("looping") );
  node.add( new BackgroundBehaviour("background") );

  /*
   * Initialise the whole infrastructure.
   */
   node.set_wifi(true);
   node.init();
   delay(500);
   Serial.println("VizBlocks Custom Behaviours Test");
}

void loop()
{
  /*
   * Then this is all we should have to do:
   */
  node.run();
}
