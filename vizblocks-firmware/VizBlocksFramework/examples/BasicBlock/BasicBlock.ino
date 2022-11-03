/*
  BasicBlock

  This is basic example code that will work with either LED or Servo VizBlocks. 
  It introduces basic block configurations and behaviours.
  
*/

#include <VizBlocks.h> // Include the VizBlocks framework library 

/*
 * Get device name: device names are set according to the unique chip ID of the device.
 */
const String device_id = String(ESP.getChipId(), HEX); // gets the chip ID
NameDictionary d;
const String name = d.get(device_id); // finds the name in our name dictionary using the chip ID
char _name[10];

/*
 * Define our VizBlock block
 */
VizBlock block(
  "null",         // Our ID (default: null)
  "VizBlocks",    // Wifi Access Point (default: VizBlocks)
  "password",     // WiFi Password (change this to the password for your VizBlocks server)
  "192.168.4.1",  // IP address of Node RED server (default: 192.168.4.1)
  1883            // Port for Node RED server (default: 1883)
  );


// Set up the Servo
Servo s1 = Servo();

// Set up the LEDs
#define LED_COUNT 12
#define LED_PIN    D2

/*
 * The LED blocks use two different types of LED ring - ether type 1, or type 2.
 * Only one of the lines below should be use (comment out the other one using //).
 * If your LED ring is displaying the wrong colours and number of LEDs then try the other type.
 */
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800); // Type 1
//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // Type 2


void setup() // This runs when the VizBlock starts up
{
  // Attach the servo to the right pin on the board
  s1.attach(D3);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);

  // Start the LED library and flash the LED ring white
  strip.begin();
  strip.fill(strip.Color(255,255,255)); // fill white
  strip.show(); // display
  delay(100); // wait 100 ms
  strip.fill(0); // fill black (off)
  strip.show(); // display

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
  block.setID(_name); // set the ID of this


 /*
  * Servo behaviours:
  */
  // Add in two different versions of the servo wiggling, with different speed parameters
  block.add(new ServoWiggle(s1, "slow_wiggle", 10) );
  block.add(new ServoWiggle(s1, "fast_wiggle", 1) );
   
  // Add in a behaviour that just goes to a certain angle, with the default name 'goto'
  block.add(new ServoGoTo(s1, "goto") );

  // Add in a behaviour that rotates from 10 to 170 degrees and back again
  block.add(new ServoGoAndReturn(s1,"drop",1, 500, 10, 170) ); // (servo, name, delay, pause, start angle, end angle)


 /*
  * LED behaviours:
  */
  // Light a specific number of leds (e.g. lighting 4 white LEDS using the command "white_leds 4")
  block.add(new LightSomeLEDs(&strip, "white_leds", strip.Color(255, 255, 255) )); // strip.Color arguments are R, G, B (0-255)
  block.add(new LightSomeLEDs(&strip, "red_leds", strip.Color(255, 47, 47) ));
  block.add(new LightSomeLEDs(&strip, "green_leds", strip.Color(56, 142, 60) ));
  block.add(new LightSomeLEDs(&strip, "blue_leds", strip.Color(13, 71, 161) ));

  // Light the entire LED ring at a given brightness (0-255) (e.g. red at 50% using the command "red 127")
  block.add(new LightAllLEDs(&strip, "white", 0, 0 )); // the last 2 arguments are hue and saturation
  block.add(new LightAllLEDs(&strip, "red", 0, 209 ));
  block.add(new LightAllLEDs(&strip, "green", 87, 156 ));
  block.add(new LightAllLEDs(&strip, "blue", 153, 235 ));

  // Light the entire LED ring with a breathing effect using a given speed (0-255) (e.g. breathe blue at full speed using the command "breathe_blue 255")
  block.add(new BreatheLEDs(&strip, "breathe_white", 0, 0 ));
  block.add(new BreatheLEDs(&strip, "breathe_red", 0, 209 ));
  block.add(new BreatheLEDs(&strip, "breathe_green", 87, 156 ));
  block.add(new BreatheLEDs(&strip, "breathe_blue", 153, 235 ));

 
  // Initialise the block
  block.set_wifi(true);
  block.init();
}


void loop() // This runs in a loop
{
  block.run();
}
