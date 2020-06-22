# README

Sometimes you want your Arduino to control AC powered devices like lamps, fans or other household devices. But because the Arduino operates at 5 volts, it cannot directly control these higher voltage devices.

That’s where the relay module comes in. You can use a relay module to control the AC mains and Arduino to control the relay.

This tutorial walks you through how to setup the one channel relay module to switch on a lamp or other device, but let’s begin with a short introduction into relays.

How Do Relays Work? A relay is an electromagnetic switch operated by a relatively small current that can control much larger current.

Here’s a simple animation illustrating how the relay uses one circuit to switch on another circuit.

relay working animation.gif Initially the first circuit is switched off and no current flows through it until something \(either a sensor or switch closing\) turns it on. The second circuit is also switched off.

When a small current flows through the first circuit, it activates the electromagnet, which generates a magnetic field all around it.

The energized electromagnet attracts a contact in the second circuit toward it, closing the switch and allowing a much bigger current to flow through the second circuit.

When the current stops flowing, the contact goes back up to its original position, switching the second circuit off again.

Relay Basics Typically the relay has 5 pins, three of them are high voltage terminals \(NC, COM, and NO\) that connect to the device you want to control.

relay pinout The mains electricity enters the relay at the common \(COM\) terminal. While use of NC & NO terminals depends upon whether you want to turn the device ON or OFF.

Between the remaining two pins \(coil1 and coil2\), there is a coil that acts like an electromagnet.

relay working animation2.gif When current flows through the coil, the electromagnet becomes charged and moves the internal contacts of the switch. At that time the normally open \(NO\) terminal connects to the common \(COM\), and the normally closed \(NC\) terminal becomes disconnected.

When current stops flowing through the coil, the internal contact returns to its initial state i.e. the normally closed \(NC\) terminal connects to the common \(COM\), and the normally open \(NO\) terminal reopens.

This is known as a single pole, double throw switch \(SPDT\). One Channel Relay Module For this tutorial, we are going to use one channel relay module. However there are other modules with two, four and eight channels. You can choose the one that best suits your needs.

one channel relay module This module is designed for switching only a single high powered device from your Arduino. It has a relay rated up to 10A per channel at 250VAC or 30VDC.

LEDs There are two LEDs on the relay module indicating the position of the relay.

one channel relay module led The Power LED will light up when the module is powered. The Status LED will light up when the relay is activated.

Output Terminal Block We have three channels of the relay broken out to blue screw pin terminals. The channels are labeled for their function: common \(COM\), normally closed \(NC\), and normally open \(NO\)

The names explain the state of the channel with relation to the switch at rest.

one channel relay module output terminal COM \(Common\): This is the pin you should connect to the signal \(mains electricity in our case\) you are planning to switch.

NC \(Normally Closed\): A normally closed configuration is used when you want to turn off the relay by default. In this configuration the relay is always closed and remains closed until you send a signal from the Arduino to the relay module to open the circuit.

NO \(Normally Open\): A normally open configuration works the other way in which the relay is always open until you send a signal from the Arduino to the relay module to close the circuit.

Control Pins On the other side of the module, there are three pins – a Ground pin and a VCC pin to power the module and an input pin IN to control the relay.

one channel relay module control pins The input pin is active low, meaning the relay will be activated when you pull the pin LOW and it will become inactive when you pull the pin HIGH.

One Channel Relay Module Pinout Let’s have a look at the pinout of one channel relay module.

one channel relay module pinout Control Pins: IN pin is used to control the relay. It is an active low pin, meaning the relay will be activated when you pull the pin LOW and it will become inactive when you pull the pin HIGH.

GND is the ground connection.

VCC pin supplies power to the module.

Output Terminal: COM pin is connected to the signal you are planning to switch.

NC pin is connected to the COM pin by default, unless you send a signal from the Arduino to the relay module to break the connection.

NO pin is open by default, unless you send a signal from the Arduino to the relay module to make the connection.

Wiring One Channel Relay Module with Arduino UNO Now that we know everything about the relay module, it is time to put it to use!

Warning: This board interacts with HIGH AC voltage. Incorrect or improper use could result in serious injury or death. So, it is intended for people experienced around, and knowledgeable about HIGH AC voltage.

Let’s set up our relay module to switch on a lamp.

Start by connecting VCC pin on the module to 5V on the Arduino and GND pin to ground. Connect the digital pin \#6 to the IN input pin for controlling the relay.

You’ll also need to place the relay module in line with the AC powered device \(lamp in our case\) you’re attempting to control. You’ll have to cut your live AC line and connect one end of the cut wire \(coming from the wall\) to COM and the other to NC or NO depending on what you want your device’s resting state to be.

If your AC device is going to be off for most of the time, and you occasionally want to turn it on, you should connect the other to NO. Connect to NC if the device will be on for most of the time.

For this project we want our lamp to be off in resting state and illuminated when we activate the relay, so we will put one end in COM and the other in NO.

The following illustration shows the wiring.

wiring one channel relay module with arduino Arduino Code Now that our hardware is all set up, let’s take a look at the code that turns the lamp on.

Here’s the simple code that will turn on the lamp for 3 seconds and keep it off for the next 3 seconds.

int RelayPin = 6;

void setup\(\) { // Set RelayPin as an output pin pinMode\(RelayPin, OUTPUT\); }

void loop\(\) { // Let's turn on the relay... digitalWrite\(RelayPin, LOW\); delay\(3000\);

```text
// Let's turn off the relay...
digitalWrite(RelayPin, HIGH);
delay(3000);
```

} Code Explanation: Controlling a relay module with the Arduino is as simple as controlling an LED. The sketch starts by declaring the pin to which input pin of relay module is connected.

int RelayPin = 6; In the setup function, we define that pin as being output.

pinMode\(RelayPin, OUTPUT\); Now in the loop function, we turn the device ON/OFF by pulling relay pin LOW/HIGH respectively.

digitalWrite\(RelayPin, LOW\) pulls the pin LOW whereas digitalWrite\(RelayPin, HIGH\) pulls the pin HIGH.

digitalWrite\(RelayPin, LOW\); delay\(3000\);

digitalWrite\(RelayPin, HIGH\); delay\(3000\);

