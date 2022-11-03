/*
  CivicDigits
  This is the basic VizBlocks sketch for used by Civid Digits for their workshops.
  It's a pretty standard configuration, which can be loaded onto servo or LED blocks and provides a
  basic set of behaviours.
  
  https://git.ecdf.ed.ac.uk/design-informatics/vizblocks/vizblocks-software/tree/master/CivicDigits
  
*/
#include <VizBlocks.h> // include the VizBlocks framework library, which can be downloaded here: 
/*
 * Get device name
 */
const String device_id = String(ESP.getChipId(), HEX); // gets the chip ID
NameDictionary d;
const String name = "bob"; // finds the name in our name dictionary using the chip ID
char _name[10];
/*
 * Define our VizBlock node
 */
VizBlock node(
  "null",       // Our ID
  "",  //Wifi Access Point
  "",  //WiFi Password
  "192.168.0.72",//IP address of comp/wifi
   1883,         //Port for Node RED server
   "CivicDigits",  //MQTT username
   "DataIsFun!",    //MQTT password
   "<F7:AD:10:85:97:1E:F8:68:08:17:A3:DC:68:F9:C3:42:DE:C9:A3:F8>" // SHA1 fingerprint (for mqtt server not needed for local)
  );
// Set up the servo we will use
Servo s1 = Servo();
// Set up LEDs as well
#define LED_COUNT 12
#define LED_PIN    D2
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // USE THIS FOR FAKE NEOPIXEL
void setup()
{
  // Attach the servo to the right pin on the board
  s1.attach(D3, 544, 4800);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
  // Start the LED library
  strip.begin();
  strip.fill(strip.Color(30,255,80));
  strip.show();
  delay(100);
  strip.fill(0);
  strip.show();
  //Get the serial port ready
  Serial.begin(9600);
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
  //Add in three different versions of the servo wiggling, with different speed parameters
  node.add(new ServoWiggle(s1, "wiggle") );
  node.add(new ServoWiggle(s1, "slow_wiggle", 10) );
  node.add(new ServoWiggle(s1, "medium wiggle", 5) );
  node.add(new ServoWiggle(s1, "fast_wiggle", 1) );
   
  //Add in a behaviour that just goes to a certain angle, with the default name 'goto'
  node.add(new ServoGoTo(s1, "goto") );
  //Add in a behaviour that rotates from 0 to 180 and back again (e.g. dropping a ball trustball style)
  node.add(new ServoGoAndReturn(s1,"drop",1,500, 3, 175) ); // (servo, name, delay, pause, start angle, end angle)
  //A few useful LED behaviours
  node.add(new LightSomeLEDs(&strip, "white_leds", strip.Color(255, 255, 255) ));
  node.add(new LightSomeLEDs(&strip, "red_leds", strip.Color(255, 47, 47) ));
  node.add(new LightSomeLEDs(&strip, "orange_leds", strip.Color(230, 81, 0) ));
  node.add(new LightSomeLEDs(&strip, "yellow_leds", strip.Color(245, 127, 23) ));
  node.add(new LightSomeLEDs(&strip, "green_leds", strip.Color(56, 142, 60) ));
  node.add(new LightSomeLEDs(&strip, "blue_leds", strip.Color(13, 71, 161) ));
  node.add(new LightSomeLEDs(&strip, "pink_leds", strip.Color(216, 27, 96) ));
  node.add(new LightSomeLEDs(&strip, "purple_leds", strip.Color(106, 27, 154) ));
  node.add(new LightSomeLEDs(&strip, "leds"));
  
  node.add(new LightAllLEDs(&strip, "white", 0, 0 ));
  node.add(new LightAllLEDs(&strip, "red", 0, 209 ));
  node.add(new LightAllLEDs(&strip, "orange", 15, 255 ));
  node.add(new LightAllLEDs(&strip, "yellow", 20, 232 ));
  node.add(new LightAllLEDs(&strip, "green", 87, 156 ));
  node.add(new LightAllLEDs(&strip, "blue", 153, 235 ));
  node.add(new LightAllLEDs(&strip, "pink", 239, 224 ));
  node.add(new LightAllLEDs(&strip, "purple", 196, 209 ));
  
  node.add(new BreatheLEDs(&strip, "breathe_white", 0, 0 ));
  node.add(new BreatheLEDs(&strip, "breathe_red", 0, 209 ));
  node.add(new BreatheLEDs(&strip, "breathe_orange", 15, 255 ));
  node.add(new BreatheLEDs(&strip, "breathe_yellow", 20, 232 ));
  node.add(new BreatheLEDs(&strip, "breathe_green", 87, 156 ));
  node.add(new BreatheLEDs(&strip, "breathe_blue", 153, 235 ));
  node.add(new BreatheLEDs(&strip, "breathe_pink", 239, 224 ));
  node.add(new BreatheLEDs(&strip, "breathe_purple", 196, 209 ));
  node.add(new BreatheLEDs(&strip, "breathe", 120, 0 ));

  //Homemade fns
  node.add(new LightHappyOrSad(&strip, "sadness"));
  node.add(new Col(&strip, "col"));
  node.add(new PulseCol(&strip, "pulsecol"));
  node.add(new BlinkCol(&strip, "blinkcol"));
 
  //Initialise the node
  node.set_wifi(true);
  node.init();
}
void loop()
{
  // Just run!
  node.run();
}
