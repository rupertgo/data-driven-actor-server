#include <VizBlocks.h>

/*
 * Class that defines a custom behaviour with a long loop that
 * can be interrupted
 */
class LoopingBehaviour : public Behaviour {
  int _num_loops = 5;
  int _loop_counter = 0;
  int _loop_length = 100;
public:
  LoopingBehaviour(String name, int num_loops=100) : Behaviour(name), _num_loops(num_loops)  {}
  String start(String args) {
    _num_loops = args.toInt();
    _loop_counter = 0;
    _running = true;
    return "Test behaviour " + _name + " with (" + args + ")";
  }

  //This is where the looping stuff actually happens
  void update() {
    Serial.print("Loop: " );
    Serial.print(_loop_counter);
    for( int i = 0; i < _loop_length; i++ ) {
      Serial.print(".");
      //do something
      delay(200);
    }
    Serial.println();
    _loop_counter++;
    if( _loop_counter > _num_loops) {
      _running = false; // This says the behaviour is finished, and it will stop
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
  node.add(new LoopingBehaviour("Loop", 50) );

  /*
   * Initialise the whole infrastructure.
   */
   node.set_wifi(true);
   node.init();
   delay(500);
   Serial.println("VizBlocks Looping Behaviour Test");
}

void loop()
{
  /*
   * Then this is all we should have to do:
   */
  node.run();
}
