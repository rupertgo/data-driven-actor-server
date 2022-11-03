#include <VizBlocks.h>

/*
 * Get device name
 */
const String device_id = String(ESP.getChipId(), HEX);
NameDictionary d;
const String name = d.get(device_id);
char _name[10];

/*
 * Set up the LEDs
 */
#define LED_COUNT 12
#define LED_PIN    D2
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

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
   * Start the LED library
   */
  strip.begin();
  strip.fill(strip.Color(30,255,80));
  strip.show();
  delay(100);
  strip.fill(0);
  strip.show();

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
   * Add in some behaviours.
   */
  node.add( new TestBehaviour("Hello") );

  node.add( new LightAllLEDs(&strip, "LightAll"));
  node.add( new LightAllLEDs(&strip, "LightAll_Red", 0, 255 ));
  node.add( new LightAllLEDs(&strip, "LightAll_Green", 120, 255 ));

  node.add( new LightSomeLEDs(&strip, "LightSome"));
  node.add( new LightSomeLEDs(&strip, "LightSome_Green", strip.Color(10, 255, 15) ) );
  node.add( new LightSomeLEDs(&strip, "LightSome_Red", strip.Color(255, 30, 40) ));

  node.add( new BreatheLEDs(&strip, "Breathe", 120, 0 ));
  node.add( new BreatheLEDs(&strip, "Breathe_Blue", 170, 255 ));

  /*
   * Initialise the whole infrastructure.
   */
  node.set_wifi(true);
  node.init();
  delay(500);
  Serial.println("VizBlocks LED Ring Test");
}

void loop()
{
  /*
   * Then this is all we should have to do:
   */
  node.run();
}
