#include <VizBlocks.h>

/*
 * Get device name
 */
const String device_id = String(ESP.getChipId(), HEX);
NameDictionary d;
const String name = d.get(device_id);
char _name[10];

/*
 * Set up the Servos
 */
Servo s1 = Servo();
Servo s2 = Servo();
Servo s3 = Servo();
Servo s4 = Servo();

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
   * Attach the servos to the right pins on the board
   */
  s1.attach(D1);
  s1.write(0);

  s2.attach(D2);
  s2.write(0);

  s3.attach(D3);
  s3.write(0);

  s4.attach(D4);
  s4.write(0);

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

  //Add in a behaviour that just goes to a certain angle, with the default name 'goto'
  node.add( new ServoGoTo(s1, "GoTo1") );
  node.add( new ServoGoTo(s2, "GoTo2") );
  node.add( new ServoGoTo(s3, "GoTo3") );
  node.add( new ServoGoTo(s4, "GoTo4") );

  //Add in a behaviour that rotates from 0 to 180 and back again (e.g. dropping a ball trustball style)
  node.add( new ServoGoAndReturn(s1,"Drop1",1,500, 3, 175) );
  node.add( new ServoGoAndReturn(s2,"Drop2",1,500, 3, 175) );
  node.add( new ServoGoAndReturn(s3,"Drop3",1,500, 3, 175) );
  node.add( new ServoGoAndReturn(s4,"Drop4",1,500, 3, 175) );

  /*
   * Initialise the whole infrastructure.
   */
  node.set_wifi(true);
  node.init();
  delay(500);
  Serial.println("VizBlocks Multi Servo Test");
}


void loop()
{
  /*
   * Then this is all we should have to do:
   */
  node.run();
}
