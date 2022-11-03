# VizBlocks Framework
This is an Arduino library that lets you easily create nodes to stick on the wall. Each node will listen on an MQTT channel, and respond to commands. So a node with a servo motor in might respond to:
* `Wiggle 30` - wiggle by 30 degrees a few times
* `GoTo 20` - move to 20 degrees
These commands can be put in via NodeRED, or through the USB Serial connection for testing. There is a growing library of `Behaviour`s that represent the different things the node might do.

[Find the project on GitLab here.](https://git.ecdf.ed.ac.uk/design-informatics/vizblocks/vizblocksframework)

## Installation
One way to install this is put it in you personal Arduino libraries directory. On a mac, this is `~/Documents/Arduino/libraries`. It is best to check it out with a capitalised name, i.e. `VizBlocksFramework` not `vizblocksframework`. To do this, you can use:

```
git clone https://git.ecdf.ed.ac.uk/design-informatics/vizblocks/vizblocksframework.git VizBlocksFramework
```

The library will then appear in your list of Arduino libraries, along with examples.

## Dependencies
This library depends on:
* Adafruit_MQTT
* Servo
* (one of the neopixel libraries once this is integrated)
* ESP8266WiFi (which should be present by magic if you're using the D1 Mini)

## Development
Because this is not a single sketch, development needs a bit of thinking about. It is probably easier to use another IDE - Atom is good! You will also have to keep pushing/pulling changes to make sure you have the latest version.

# Usage
The library is designed to be used within normal Arduino sketches. It wraps up most of the business of connecting to MQTT etc. in a `VizBlock` class, and you then give it a bunch of `Behaviour`s that do interesting things.

## Setup
The node is set up with all of the connection information for the WiFi network and for Node-RED, e.g.:

```arduino
#include <VizBlocks.h>

VizBlocks node(
  "new001",     // Our ID
  "VizBlocksNet", //Wifi Access Point
  "VizBlocksAP",  //WiFi Password
  "192.168.4.1",//IP address of Node RED server
  1883          //Port for Node RED server
  );
```

A very simple sketch would then simply initialise this, and call its main function in a loop:

```
void setup()
{
  Serial.begin(115200);
  //Initialise the whole infrastructure
  node.init();
}

void loop()
{
  node.run();
}

```

## Adding behaviours
However, this would not do anything useful - it needs to know what to do. So we should add some `Behaviour`s. Each behaviour is one thing that the node knows how to do - set a number of LEDs to be active, move a servo to a certain angle, wiggle a servo for a bit etc.

Any number of behaviours can be added. They each need a unique name, so that they can be called correctly. Behaviours can also have parameters. In this example, we are adding three different behaviours, that all wiggle a servo. They have different speeds of wiggling, though (the last parameter) and different names. (`s1` is the Servo object they are working on).

```
node.add( new ServoWiggle(s1, "Wiggle") );
node.add( new ServoWiggle(s1, "Wiggle_Slow", 10) );
node.add( new ServoWiggle(s1, "Wiggle_Fast", 1) );
```

Now, the node will respond to three commands - `Wiggle`, `Wiggle_Slow` and `Wiggle_Fast`.

You can see a full example of this in the `ServoMotorNode` sketch.

## Calling Behaviours
Behaviours can currently be called in two ways:
* via Node RED, by sending a message on the channel. So, instead of sending '100' to make the node wiggle, now you have to send 'wiggle 100'.
* you can also do this using the Arduino serial monitor, to make it easier to test

Each `Behaviour` has a unique name, and some number of parameters (mostly none, or one).

# Behaviours
Behaviours are defined as classes. Each class has a unique name, and a constructor that takes several arguments. These are for things that should not change, e.g. the Servo object that is being wiggled. When the behaviour is called, it is called with arguments for *just that action*. So, for wiggling a servo, the constructor is:

`ServoWiggle( <servo_object>, <name>, <wiggle_speed> )`

When the behaviour is then called, it is given the angle through which to wiggle, so the size of the wiggle can be controlled. So if the name is `Wiggle`, it would be called by sending:

`Wiggle 40`

The current set of behaviours and their parameters are:

| Constructor | Called with | Effect |
| ---- | ---- | ---- |
|`ServoWiggle( <servo_object>, <name>, <wiggle_speed> )`|`<name> <int wiggle_angle>`|wiggles the servo|
|`ServoWiggle( <servo_object>, <name>, <wiggle_speed> )`|`<name> <int wiggle_angle>`|wiggles the servo|
|`ServoGoto( <servo_object>, <name> )`|`<name> <int angle>`|Moves the servo to that angle|

The next ones that are about to be written are something like the following:

| Constructor | Called with | Effect |
| ---- | ---- | ---- |
|`LEDNumber( <led_object>, <name>, <color>)`|`<name> <int num_leds>`|lights up that many LEDs on the object in the color given|
|`LEDSet( <led_object>, <name> )`|`<name> <int hue> <int brightness>`|Lights up the whole strip with the hue and brightness supplied|
|`ContinuousServo( <servo_object>, <name>, <speed> )`|`<name> <int time>`|Moves the continuous servo at the given speed for the given time. A negative time means move the other way.|
|`BallDrop( <servo_object>, <name>, <speed> )`|`<name>`|Drops a single ball. The <speed> parameter or something similar tunes it to work with different mechanisms.|
|`LinearSlide( <slide_object>, <name> )`|`<name> <int position>`|Moves the linear slide to the given position. Position should be consistent for different slides, so maybe a value from 1.1000 mapped to the length of the slide.|

_Please update this table as you generate new behaviours!_

## Creating behaviours
At first - make behaviours with your sketch, while you are testing them. See the examples for this!

When you've got something stable, add it to the library. If it fits within an existing file, add it to that (e.g. if it's another thing that Servos do, add it to `ServoBehaviours.h`). If it's a new kind of thing, then create a new file and add it in there. (Make sure you do the #IFNDEF stuff - ask if you don't know what that is!).

## Some commands from the examples:
Node Red commands for Evan's blocks:

Servos:
* `Wiggle <int>` - int between 0 - 180
* `Slow_Wiggle <int>` - int between 0 - 180
* `Fast_Wiggle <int>` - int between 0 - 180
* `GoTo <int>` - int between 0 - 180
* `Drop` - behaviour that rotates from 0 to 180 and back again

LEDs:
* `LightSome_Green <int>` - turns LEDs green, int between 0 - 12
* `LightSome_Red <int>` - turns LEDs red,  int between 0 - 12
* `LightSome <int>` - turns LEDs white,  int between 0 - 12
* `LightAll_Green <int>` - turns all LEDs green with set brightness, int between 0 - 100 ?
* `LightAll_Red <int>` - turns all LEDs red with set brightness, int between 0 - 100 ?
* `Breathe <int>` - LEDs increase / decrease brightness with set speed, int between ?
* `Breathe_Blue <int>` - blue LEDs increase / decrease brightness with set speed, int between ?
