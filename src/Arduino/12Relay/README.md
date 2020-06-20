# SOLID STATE RELAY
###### tags: `eris-devkit`

## INTRODUCTION
Sometimes you want your micro controller to control AC powered devices like lamps, fans or other household devices. But because the micro controller operates at 5 volts, it cannot directly control these higher voltage devices. That’s where the relay module comes in. You can use a relay module to control the AC mains and micro controller to control the relay.

## BACKGROUND INFO
We will walk through how to setup the one channel relay module to switch on a lamp or other device.

#### How Do Relays Work?
A relay is an electromagnetic switch operated by a relatively small current that can control much larger current.

Initially the first circuit is switched off and no current flows through it until something (either a sensor or switch closing) turns it on. The second circuit is also switched off.

When a small current flows through the first circuit, it activates the electromagnet, which generates a magnetic field all around it.

The energized electromagnet attracts a contact in the second circuit toward it, closing the switch and allowing a much bigger current to flow through the second circuit.

When the current stops flowing, the contact goes back up to its original position, switching the second circuit off again.

#### Relay Basics
Typically the relay has 5 pins, three of them are high voltage terminals (NC, COM, and NO) that connect to the device you want to control.

The mains electricity enters the relay at the common (COM) terminal. While use of NC & NO terminals depends upon whether you want to turn the device ON or OFF.

When current flows through the coil, the electromagnet becomes charged and moves the internal contacts of the switch. At that time the normally open (NO) terminal connects to the common (COM), and the normally closed (NC) terminal becomes disconnected.

When current stops flowing through the coil, the internal contact returns to its initial state i.e. the normally closed (NC) terminal connects to the common (COM), and the normally open (NO) terminal reopens.

- **COM (Common)**: This is the pin you should connect to the signal (mains electricity in our case) you are planning to switch.

- **NC (Normally Closed)**: A normally closed configuration is used when you want to turn off the relay by default. In this configuration the relay is always closed and remains closed until you send a signal from the eris dev kit to the relay module to open the circuit.

- **NO (Normally Open)**: A normally open configuration works the other way in which the relay is always open until you send a signal from the eris dev kit to the relay module to close the circuit.

- **Control** transmit data
- **GND** is the ground connection.
- **VCC** pin supplies power to the module.

#### Pinout

Generic relay

![](https://i.imgur.com/8NuMAID.png)

How it looks like

![](https://i.imgur.com/XrJftak.jpg)


**Warning:
This board interacts with HIGH AC voltage. Incorrect or improper use could result in serious injury or death. So, it is intended for people experienced around, and knowledgeable about HIGH AC voltage.**

## PROCEDURE
#### How to setup relay

Start by connecting VCC pin on the module to 5V on the eris-dev-kit and GND pin to ground. Connect the pin PA6 to the signal pin for controlling the relay.

You’ll also need to place the relay module in line with the AC powered device (lamp in our case) you’re attempting to control. You’ll have to cut your live AC line and connect one end of the cut wire (coming from the wall) to COM and the other to NC or NO depending on what you want your device’s resting state to be.

If your AC device is going to be off for most of the time, and you occasionally want to turn it on, you should connect the other to NO. Connect to NC if the device will be on for most of the time.

#### Programming
```cpp
int RelayPin = PA6;//declaring the pin to which input pin of relay module is connected.

void setup() {
	// Set RelayPin as an output pin
	pinMode(RelayPin, OUTPUT);
}

void loop() {
	// Let's turn on the relay...
	digitalWrite(RelayPin, LOW);//pulls the pin LOW
	delay(3000);
	
	// Let's turn off the relay...
	digitalWrite(RelayPin, HIGH);//pulls the pin HIGH.
	delay(3000);
}
```
## APPLICATIONS
- home automation
- farm or garden monitoring systems.
