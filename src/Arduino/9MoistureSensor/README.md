In this tutorial we will use one soil hygrometer module to measure soil moisture of a pot.

The module can give us a digital signal when the soil need watering and this output can be adjusted by the potentiometer. Or it can give us an analog signal of current soil moisture!

In this tutorial we will use the analog signal output of this module and we will change it in percentage value. Finally we will print in serial monitor the current percentage value of soil moisture.

So, let's get started!

Add TipAsk QuestionCommentDownload
Step 1: What You Will Need
For this tutorial you will need:

Arduino uno
Breadboard
Soil hygrometer module
some breadboard cables
And some soil for testing

Add TipAsk QuestionCommentDownload
Step 2: The Circuit
The Circuit
The connections are pretty easy, see the above image with the breadboard circuit schematic.

Add TipAsk QuestionCommentDownload
Step 3: The Code
Here's the code, embedded using Codebender!

Try downloading the Codebender plugin and clicking on the "Run on Arduino" button to program your Arduino board. And that's it, you've programmed your Arduino with this sketch!



You can keep playing with that by clicking the "Edit" button and start making your own modifications to the code.

Add TipAsk QuestionCommentDownload
Step 4: Serial Monitor - Testing
Serial Monitor - Testing
Put the sensor in dry soil for testing. Press the connect button below to start the serial communication.

Now throw some water on the soil and observe how the value changes



Add TipAsk QuestionCommentDownload
Step 5: Well Done!
You have successfully completed one more "How to" tutorial and you learned how to use the saoil hygrometer module with Arduino.

Check this Arduino Automatic Watering System

I hope you liked this, let me know in the comments!

Add TipAsk QuestionCommentDownload
1 Person Made This Project!
How to Use the Soil Hygrometer Module - Arduino TutorialNadavZ made it!
Did you make this project? Share it with us!

I Made It!
Recommendations
DIY PCB Shaker (Low-Cost)
DIY PCB Shaker (Low-Cost) by ASCAS in Tools
 107  8.8K
Arduino Magnetic Board 
Arduino Magnetic Board by andrei.erdei in Arduino
 55  11K
See-Through Arduino UNO
See-Through Arduino UNO by jiripraus in Arduino
 403  60K
Backyard ContestBackyard Contest
Arduino Contest 2020Arduino Contest 2020
First Time Author ContestFirst Time Author Contest

Add Tip
Ask Question
Post Comment
We have a be nice policy.
Please be positive and constructive.

Add Images
Post
6 Discussions
MattM422
MattM422Question 1 year ago
AnswerUpvote
Out of curiosity, why did you constrain the hygrometer's analog value to a set of numbers and then map it to yet another set of numbers? Why not just deal with the analog value directly?

britdub
britdub3 years ago
ReplyUpvote
Hello again I'm trying to add a pump to this project operated via a relay on pin A1

but I'm having problems with the value being stuck at 100% after adding the lines

to switch the relay

/* Arduino Tutorial - How to use a Soil Hygrometer Module

Dev: Michalis Vasilakis // Date: 18/05/2016 // www.ardumotive.com */

//Constants

const int hygrometer = A0; //Hygrometer sensor analog pin output at pin A0 of Arduino

int pump = A1; //pump on off

//Variables

int value;

void setup(){

pinMode (pump, OUTPUT); //SETS PIN A1 as output

Serial.begin(9600);

}

void loop(){

// When the plant is watered well the sensor will read a value 380~400, I will keep the 400

// value but if you want you can change it below.

value = analogRead(hygrometer); //Read analog value

value = constrain(value,300,160); //Keep the ranges!

value = map(value,300,160,100,0); //Map value : 400 will be 100 and 1023 will be 0

// if (value = 0) digitalWrite(pump, HIGH); // sets the pump on

// if (value = 100) digitalWrite(pump, LOW); // sets the pump off

Serial.print("Soil humidity: ");

Serial.print(value);

Serial.println("%");

delay(2000); //Read every 2 sec.

}

1 reply 
britdub
britdub3 years ago
ReplyUpvote
hello I'm a noob and I'm having problems with errors

(sketch file) [arduino-tutorial] Soil hygrometer module.ino:25:17:error: expected expression
Serial.println(%);
^
(sketch file) [arduino-tutorial] Soil hygrometer module.ino:25:18:error: expected expression
Serial.println(%);
^
2 errors generated.

I'm using a arduino uno connected via usb

Regards

Stevie p


In this article, we are going to interface a Soil Moisture Sensor with Arduino Uno. We will measure the moisture content of soil in pot or farming field and by using this we can pour water into field based on preset values.

This sensor measures the volumetric content of water inside the soil (based on resistance/conductivity) and gives the moisture level as output. The sensor is equipped with both analog and digital output, so it can be used in both analog and digital mode.

Moisture Sensor
Soil moisture is basically the content of water present in the soil. The soil moisture sensor consists of two conducting plates which function as a probe. This sensor is used to measure the volumetric content of water.

It can measure the moisture content in the soil based on the change in resistance between these two conducting plates. It uses capacitance to measure the dielectric constant of the soil. Dielectric constant can be called as the ability of soil to transmit electricity.


When there is more water, the soil will conduct more electricity which means that there will be less resistance. Therefore, the moisture level will be higher.

Similarly dry soil conducts less electricity. When there will be less water, then the soil will conduct less electricity which means that there will be more resistance. Therefore, the moisture level will be lower.

Specifications
The specifications of the soil moisture sensor FC-28 are as follows

Parameter	Value
Input Voltage	3.3 – 5V
Output Voltage	0 – 4.2V
Input Current	35mA
Output Signal	Both Analog (A0) and Digital(D0)
The Module also contains a potentiometer which will set the threshold value and then this threshold value will be compared by the LM393 comparator which is used for digital output. The output LED will light according to this threshold value.

Operating Modes
As explained above this sensor can be connected in two modes: Analog Mode and Digital Mode. First, we will connect it in Analog mode and then we will use it in Digital mode.

Analog Mode
Circuit Diagram
To make it to operate in Analog Mode, we need to connect analog output of the sensor to ADC input of the Arduino. While monitoring the sensor we will get ADC values from 0-1023. The moisture content will be measured in percentage, so we have to map the values from 0 -100 and then that will be displayed on Serial Monitor.

We can further set different ranges of the moisture values and turn on or off the water pump according to it.

Interfacing Soil Moisture Sensor with Arduino – Circuit Diagram

Software
Arduino Code
int sensor_pin = A0;
int output_value ;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Reading From the Sensor ...");
  delay(2000);
}

void loop() 
{
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,550,0,0,100);
  Serial.print("Mositure : ");
  Serial.print(output_value);
  Serial.println("%");
  delay(1000);
}
Code Explanation
Here we have defined two variables; one for the soil moisture sensor pin and the other for storing the output of the sensor.

int sensor_pin = A0; // Soil Sensor input at Analog PIN A0
int output_value ;
In the setup function, the “Serial.begin(9600)” command we are initializing the serial port of Arduino to display information in the serial monitor. Then, we will print the “Reading From the Sensor …” on the serial monitor.

void setup()
{
  Serial.begin(9600);
  Serial.println("Reading From the Sensor ...");
  delay(2000);
}
In the loop function, we will read from the sensor analog pin and will store the values in the “output_ value” variable. Then, we will map the output values to 0-100, because the moisture is measured in percentage. When we took the readings from the dry soil, then the sensor value was 550 and in the wet soil, the sensor value was 10. So, we mapped these values to get the moisture. After that, we printed these values on the serial monitor.

void loop() 
{
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,550,0,0,100);
  Serial.print("Mositure : ");
  Serial.print(output_value);
  Serial.println("%");
  delay(1000);
}
Practical Implementation
Interfacing Moisture Sensor with Arduino Analog Mode – Practical Implementation

Digital Mode
To connect the soil moisture sensor FC-28 in the digital mode, we have to use digital output of the sensor and a digital input of Arduino. While monitoring the sensor gives us true or false condition with respect to measured moisture content in a soil. We can adjust the threshold using the potentiometer in the sensor.

When the sensed value is less than the threshold value, then the measured soil moisture content is low and the sensor will give us LOW.

When the sensed value is greater than the threshold value, then the measured soil moisture content is high and the sensor will give us HIGH.

Circuit Diagram
The connections for connecting the soil moisture sensor FC-28 to the Arduino in digital mode are as follows.

VCC of Moisture Sensor to 5V of Arduino
GND of Moisture Sensor to GND of Arduino
D0 of Moisture Sensor to pin 8 of Arduino
LED1 positive to pin 13 of Arduino and negative to GND of Arduino
LED2 positive to pin 12 of Arduino and negative to GND of Arduino
 

Interfacing Moisture Sensor with Arduino Digital Mode – Circuit Diagram

Software
Arduino Code
int led_pin1 =13;
int led_pin2 =12;
int sensor_pin =8;

void setup()
{
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(sensor_pin, INPUT);
}

void loop() 
{
  if(digitalRead(sensor_pin) == HIGH)
    {
      digitalWrite(led_pin1, HIGH);
      digitalWrite(led_pin2, LOW);
      delay(2000);
    }
  else
    {
      digitalWrite(led_pin1, LOW);
      digitalWrite(led_pin2, HIGH);
      delay(2000);
    }
}
Code Explanation
First of all, we have initialized two variable for connecting the LED pin and the sensor digital pin.

int led_pin1 =13;
int led_pin2 =12;
int sensor_pin =8;
In the setup function, we have declared the LED pin as the output pin because; we will power the LED through that pin. Then, we declared the sensor pin as an input pin because the Arduino will take the values from the sensor through that pin.

void setup()
{
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(sensor_pin, INPUT);
}
In the loop function, we have read from the sensor pin. If the output value of the sensor is higher than the threshold value, then the digital pin will be high and the led_pin1 will light up and led_pin2 will go off. If the sensor value is lower than the threshold value, then the led_pin1 will go off and led_pin2 will light up.

void loop() 
{
  if(digitalRead(sensor_pin) == HIGH)
    {
      digitalWrite(led_pin1, HIGH);
      digitalWrite(led_pin2, LOW);
      delay(2000);
    }
  else
    {
      digitalWrite(led_pin1, LOW);
      digitalWrite(led_pin2, HIGH);
      delay(2000);
    }
}
Practical Implementation
Interfacing Moisture Sensor with Arduino-Digital Mode- Practical Implementation

Video


Applications
Agriculture Department
Landscape Irrigation
Gardening purpose



How Soil Moisture Sensor Works and Interface it with Arduino
Tutorial for Interfacing Soil Moisture Sensor with Arduino
When you hear the term ‘smart garden’, one of the things that comes to your mind is a system that measures soil moisture and irrigates your plants automatically.

With this type of system, you can water your plants only when needed and avoid over-watering or under-watering.

If you want to build such a system then you will definitely need a Soil Moisture Sensor.

How Soil Moisture Sensor works?
The working of the soil moisture sensor is pretty straightforward.

The fork-shaped probe with two exposed conductors, acts as a variable resistor (just like a potentiometer) whose resistance varies according to the water content in the soil.

soil moisture sensor working.gif
This resistance is inversely proportional to the soil moisture:

The more water in the soil means better conductivity and will result in a lower resistance.
The less water in the soil means poor conductivity and will result in a higher resistance.
The sensor produces an output voltage according to the resistance, which by measuring we can determine the moisture level.

Hardware Overview
A typical soil moisture sensor has two components.

The Probe
The sensor contains a fork-shaped probe with two exposed conductors that goes into the soil or anywhere else where the water content is to be measured.

Like said before, it acts as a variable resistor whose resistance varies according to the soil moisture.

soil moisture sensor probe
The Module
The sensor also contains an electronic module that connects the probe to the Arduino.

The module produces an output voltage according to the resistance of the probe and is made available at an Analog Output (AO) pin.

The same signal is fed to a LM393 High Precision Comparator to digitize it and is made available at an Digital Output (DO) pin.

soil moisture sensor sensitivity adjustment
The module has a built-in potentiometer for sensitivity adjustment of the digital output (DO).

You can set a threshold by using a potentiometer; So that when the moisture level exceeds the threshold value, the module will output LOW otherwise HIGH.

This setup is very useful when you want to trigger an action when certain threshold is reached. For example, when the moisture level in the soil crosses a threshold, you can activate a relay to start pumping water. You got the idea!

Tip: Rotate the knob clockwise to increase sensitivity and counterclockwise to decrease it.

soil moisture sensor power and status leds
Apart from this, the module has two LEDs. The Power LED will light up when the module is powered. The Status LED will light up when the digital output goes LOW.

Soil Moisture Sensor Pinout
The soil moisture sensor is super easy to use and only has 4 pins to connect.

soil moisture sensor pinout
AO (Analog Output) pin gives us an analog signal between the supply value to 0V and will be connected to one of the analog inputs on your Arduino.

DO (Digital Output) pin gives Digital output of internal comparator circuit. You can connect it to any digital pin on an Arduino or directly to a 5V relay or similar device.

VCC pin supplies power for the sensor. It is recommended to power the sensor with between 3.3V – 5V. Please note that the analog output will vary depending on what voltage is provided for the sensor.

GND is a ground connection.

Sensing Soil Moisture using Analog Output
As you know that the module provides both analog and digital output, so for our first experiment we will measure the soil moisture by reading the analog output.

Wiring
Let’s hook the soil moisture sensor up to the Arduino.

First you need to supply power to the sensor. For that you may connect the VCC pin on the module to 5V on the Arduino.

However, one commonly known issue with these sensors is their short lifespan when exposed to a moist environment. Having power applied to the probe constantly speeds the rate of corrosion significantly.

To overcome this, we recommend that you do not power the sensor constantly, but power it only when you take the readings.

An easy way to accomplish this is to connect the VCC pin to a digital pin of an Arduino and set it to HIGH or LOW as per your requirement.

Also the total power drawn by the module (with both LEDs lit) is about 8 mA, so it is okay to power the module off a digital pin on an Arduino.

So, let’s connect the VCC pin on the module to the digital pin #7 of an Arduino and GND pin to ground.

Finally, connect the AO pin on the module to the A0 ADC pin on your Arduino.

The following illustration shows the wiring.

wiring soil moisture sensor with arduino for reading analog output
Calibration
To get accurate readings out of your soil moisture sensor, it is recommended that you first calibrate it for the particular type of soil that you plan to monitor.

Different types of soil can affect the sensor, so your sensor may be more or less sensitive depending on the type of soil you use.

Before you start storing data or triggering events, you should see what readings you are actually getting from your sensor.

Use the below sketch to note what values your sensor outputs when the soil is as dry as possible -vs- when it is completely saturated with moisture.

// Sensor pins
#define sensorPower 7
#define sensorPin A0

void setup() {
	pinMode(sensorPower, OUTPUT);
	
	// Initially keep the sensor OFF
	digitalWrite(sensorPower, LOW);
	
	Serial.begin(9600);
}

void loop() {
	//get the reading from the function below and print it
	Serial.print("Analog output: ");
	Serial.println(readSensor());
	
	delay(1000);
}

//  This function returns the analog soil moisture measurement
int readSensor() {
	digitalWrite(sensorPower, HIGH);	// Turn the sensor ON
	delay(10);							// Allow power to settle
	int val = analogRead(sensorPin);	// Read the analog value form sensor
	digitalWrite(sensorPower, LOW);		// Turn the sensor OFF
	return val;							// Return analog moisture value
}
When you run the sketch, you’ll see the close to the following readings in the serial monitor:

when the soil was dry (~850)
when the soil was completely wet (~400)
calibrating soil moisture sensor
This test may take some trial and error. Once you get a good handle on these readings, you can use them as threshold if you intend to trigger an action.

Final Build
Based on the calibration values, the program below defines the following ranges to determine the status of the soil:

< 500 is too wet
500-750 is the target range
> 750 is dry enough to be watered
/* Change these values based on your calibration values */
#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'

// Sensor pins
#define sensorPower 7
#define sensorPin A0

void setup() {
	pinMode(sensorPower, OUTPUT);
	
	// Initially keep the sensor OFF
	digitalWrite(sensorPower, LOW);
	
	Serial.begin(9600);
}

void loop() {
	//get the reading from the function below and print it
	int moisture = readSensor();
	Serial.print("Analog Output: ");
	Serial.println(moisture);

	// Determine status of our soil
	if (moisture < soilWet) {
		Serial.println("Status: Soil is too wet");
	} else if (moisture >= soilWet && moisture < soilDry) {
		Serial.println("Status: Soil moisture is perfect");
	} else {
		Serial.println("Status: Soil is too dry - time to water!");
	}
	
	delay(1000);	// Take a reading every second for testing
					// Normally you should take reading perhaps once or twice a day
	Serial.println();
}

//  This function returns the analog soil moisture measurement
int readSensor() {
	digitalWrite(sensorPower, HIGH);	// Turn the sensor ON
	delay(10);							// Allow power to settle
	int val = analogRead(sensorPin);	// Read the analog value form sensor
	digitalWrite(sensorPower, LOW);		// Turn the sensor OFF
	return val;							// Return analog moisture value
}
If everything is fine, you should see below output on serial monitor.

soil moisture sensor analog output
Sensing Soil Moisture using Digital Output
For our second experiment we will determine the status of the soil by using the digital output.

Wiring
We’ll use the circuit from the previous example. This time we just need to remove the connection to ADC pin and connect DO pin on the module to the digital pin #8 on the Arduino.

Hook up your circuit as pictured below:

wiring soil moisture sensor with arduino for reading digital output
Calibration
The module has a built-in potentiometer for calibrating the digital output (DO).

By turning the knob of the potentiometer, you can set a threshold. So that when the moisture level exceeds the threshold value, the Status LED will light up and the module will output LOW.

digital output of soil moisture sensor
Now to calibrate the sensor, insert the probe into the soil when your plant is ready to be watered and adjust the pot clockwise so that the Status LED is ON and then adjust the pot back counterclockwise just until the LED goes OFF.

That’s it your sensor is now calibrated and ready for use.

Arduino Code
Once the circuit is built, upload the following sketch to your Arduino.

// Sensor pins
#define sensorPower 7
#define sensorPin 8

void setup() {
	pinMode(sensorPower, OUTPUT);

	// Initially keep the sensor OFF
	digitalWrite(sensorPower, LOW);

	Serial.begin(9600);
}

void loop() {
	//get the reading from the function below and print it
	int val = readSensor();
	Serial.print("Digital Output: ");
	Serial.println(val);

	// Determine status of our soil moisture situation
	if (val) {
		Serial.println("Status: Soil is too dry - time to water!");
	} else {
		Serial.println("Status: Soil moisture is perfect");
	}

	delay(1000);	// Take a reading every second for testing
					// Normally you shoul take reading perhaps every 12 hours
	Serial.println();
}

//  This function returns the analog soil moisture measurement
int readSensor() {
	digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
	delay(10);              // Allow power to settle
	int val = digitalRead(sensorPin); // Read the analog value form sensor
	digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
	return val;             // Return analog moisture value
}
If everything is fine, you should see below output on serial monitor.

