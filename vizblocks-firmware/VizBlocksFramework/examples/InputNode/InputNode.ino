/*
 * This is quite complicated as it stacks all the inputs on top of each other.
 * It would be best to split this into individual sketches for a button, a
 * potentiometer and an encoder.
 */

#include "VizBlocks.h"
#include "Button.h"
#include "Potentiometer.h"
#include "RotaryEncoder.h"

/*
 * Get device name
 */
const String device_id = String(ESP.getChipId(), HEX);
NameDictionary d;
const String name = d.get(device_id);
char _name[10];

/*
 * Set up the Button
 */
const int bPin = 2;
const int bID = 0;
Button b(bPin, bID);

/*
 * Set up the Potentiometer
 */
const int pPin = A0;
const int pID = 1;
Potentiometer p(pPin, pID);

/*
 * Set up the RotaryEncoder
 */
const int rePinA = 4;
const int rePinB = 5;
const int reID = 2;
RotaryEncoder re(rePinA, rePinB, reID);

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

/*
 * Interrupt Serivce Routines
 */
void ICACHE_RAM_ATTR bISR() {
  b.tick();
}

void ICACHE_RAM_ATTR reISR() {
  re.tick();
}

void setup() {
  /*
   * Get the serial port ready.
   */
  Serial.begin(115200);
  Serial.println("Serial started!");
  delay(500);

  /*
   * Pass event handlers into button
   */
  b.initInterrupts(bISR);
  b.setEventHandler(bCB);

  /*
   * Pass event handlers into potentiometer
   */
  p.setEventHandler(pCB);

  /*
   * Pass event handlers into rotary encoder
   */
  re.initInterrupts(reISR);
  re.setEventHandler(reCB);

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
  node.add(new SendCapabilities(&node) );
  node.add(new ButtonClicked(&node, "ButtonClicked") );
  node.add(new ButtonHeld(&node, "ButtonHeld") );
  node.add(new ButtonTick(&node, "ButtonTick") );
  node.add(new ButtonReleased(&node, "ButtonReleased") );
  node.add(new PotentiometerUpdated(&node, "PotentiometerUpdated") );
  node.add(new RotaryEncoderUpdated(&node, "RotaryEncoderUpdated") );


  /*
   * Initialise the whole infrastructure.
   */
  node.set_wifi(true);
  node.init();
  delay(500);
  Serial.println("VizBlocks Inputs Test");
}

void loop() {
  node.run();
  b.check();
  p.check();
  re.check();
}

void bCB(Button* button, uint8_t eventType, bool state) {
  /*
   * Button event handler that triggers VizBlock behaviours
   */

  String idString = String(button->getId());

  Serial.println("Button ID: " + idString + " Event Type: " + String(eventType) + " State: " + String(state));

  switch(eventType) {
    case Button::kEventPressed:
    //Do something
    break;

    case Button::kEventReleased:
    //Do something else
    node.process("ButtonReleased " + idString);
    break;

    case Button::kEventClicked:
    //Do something else
    node.process("ButtonClicked " + idString);
    break;

    case Button::kEventHeld:
    //Do something else
    node.process("ButtonHeld " + idString);
    break;

    case Button::kEventTick:
    //Do something else
    node.process("ButtonTick " + idString);
    break;
  }
}

void pCB(Potentiometer* potentiometer, uint8_t eventType, uint8_t sensorValue) {
  /*
   * Potentiometer Event Handler that triggers VizBlock behaviours
   */

  String idString = String(potentiometer->getId());

  Serial.println("Slider ID: " + idString + " Event Type: " + String(eventType) + " Sensor Value: " + String(sensorValue));

  switch(eventType) {
    case Potentiometer::kEventStableUpdate:
    //Do something
    node.process("PotentiometerUpdated " + idString + " " + String(sensorValue));
    break;

    case Potentiometer::kEventUnstableUpdate:
    //Do something else
    break;
  }
}

void reCB(RotaryEncoder* rotaryEncoder, uint8_t eventType, int position) {
  /*
   * Rotary Encoder event handler that triggers VizBlock behaviours
   */

  String idString = String(rotaryEncoder->getId());

  Serial.println("Encoder ID: " + idString + " Event Type: " + String(eventType) + " Position: " + String(position));

  switch(eventType) {
    case RotaryEncoder::kEventStableUpdate:
    //Do something
    node.process("RotaryEncoderUpdated " + idString + " " + String(position));
    break;

    case RotaryEncoder::kEventUnstableUpdate:
    //Do something else
    break;
  }
}
