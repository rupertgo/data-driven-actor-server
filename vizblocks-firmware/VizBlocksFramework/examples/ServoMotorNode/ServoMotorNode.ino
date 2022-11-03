#include <VizBlocks.h>

/*
 * Get device name
 */
const String device_id = String(ESP.getChipId(), HEX);
NameDictionary d;
const String name = d.get(device_id);
char _name[10];

/*
 * Set up the Servo
 */
Servo s1 = Servo();

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
   * Attach the servo to the right pin on the board
   */
  s1.attach(D3);
  s1.write(0);

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
   * Add in some behaviours:
   */

  //Add in a test behaviour that responds to the command 'hello'
  node.add( new TestBehaviour("Hello") );

  //Add in three different versions of the servo wiggling, with different speed parameters
  node.add( new ServoWiggle(s1, "Wiggle") );
  node.add( new ServoWiggle(s1, "Wiggle_Slow", 10) );
  node.add( new ServoWiggle(s1, "Wiggle_Fast", 1) );

  //Add in a behaviour that just goes to a certain angle
  node.add( new ServoGoTo(s1, "GoTo") );

  //Add in a behaviour that rotates from 0 to 180 and back again (e.g. dropping a sweet gumball machine style)
  node.add( new ServoGoAndReturn(s1,"GoAndReturn_Drop", 1, 500, 3, 175) );

  /*
   * Initialise the whole infrastructure.
   */
  node.set_wifi(true);
  node.init();
  delay(500);
  Serial.println("VizBlocks Servo Motor Test");
}

void loop()
{
  /*
   * Then this is all we should have to do:
   */
  node.run();
}
