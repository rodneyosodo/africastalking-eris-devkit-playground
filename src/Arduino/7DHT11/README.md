The DHT11 humidity and temperature sensor makes it really easy to add humidity and temperature data to your DIY electronics projects. It’s perfect for remote weather stations, home environmental control systems, and farm or garden monitoring systems.

PCBWay Ad
In this tutorial, I’ll first go into a little background about humidity, then I’ll explain how the DHT11 measures humidity. After that, I’ll show you how to connect the DHT11 to an Arduino and give you some example code so you can use the DHT11 in your own projects.

BONUS: I made a quick start guide for this tutorial that you can download and go back to later if you can’t set this up right now. It covers all of the steps, diagrams, and code you need to get started.
Here are the ranges and accuracy of the DHT11:

Humidity Range: 20-90% RH
Humidity Accuracy: ±5% RH
Temperature Range: 0-50 °C
Temperature Accuracy: ±2% °C
Operating Voltage: 3V to 5.5V
The DHT11 Datasheet:

Circuit Basics PDF Icon DHT11 Datasheet

WHAT IS RELATIVE HUMIDITY?
The DHT11 measures relative humidity. Relative humidity is the amount of water vapor in air vs. the saturation point of water vapor in air. At the saturation point, water vapor starts to condense and accumulate on surfaces forming dew.

The saturation point changes with air temperature. Cold air can hold less water vapor before it becomes saturated, and hot air can hold more water vapor before it becomes saturated.

The formula to calculate relative humidity is:

RH = (\frac{\rho_{w}}{\rho_{s}}) \ x \ 100 \% \\ \\ RH: \ Relative \ Humidity \\ \rho_{w}: \ Density \ of \ water \ vapor\\ \rho_{s}: \ Density \ of \ water \ vapor \ at \ saturation 

Relative humidity is expressed as a percentage. At 100% RH, condensation occurs, and at 0% RH, the air is completely dry.


HOW THE DHT11 MEASURES HUMIDITY AND TEMPERATURE
The DHT11 detects water vapor by measuring the electrical resistance between two electrodes. The humidity sensing component is a moisture holding substrate with electrodes applied to the surface. When water vapor is absorbed by the substrate, ions are released by the substrate which increases the conductivity between the electrodes. The change in resistance between the two electrodes is proportional to the relative humidity. Higher relative humidity decreases the resistance between the electrodes, while lower relative humidity increases the resistance between the electrodes.

The DHT11 measures temperature with a surface mounted NTC temperature sensor (thermistor) built into the unit. To learn more about how thermistors work and how to use them on the Arduino, check out our Arduino Thermistor Temperature Sensor Tutorial.

With the plastic housing removed, you can see the electrodes applied to the substrate:

DHT11 Temperature and Humidity Sensor Inside Front with Cover Removed

An IC mounted on the back of the unit converts the resistance measurement to relative humidity. It also stores the calibration coefficients, and controls the data signal transmission between the DHT11 and the Arduino:

DHT11 Temperature and Humidity Sensor Inside Back with Cover Removed

The DHT11 uses just one signal wire to transmit data to the Arduino. Power comes from separate 5V and ground wires. A 10K Ohm pull-up resistor is needed between the signal line and 5V line to make sure the signal level stays high by default (see the datasheet for more info).

There are two different versions of the DHT11 you might come across. One type has four pins, and the other type has three pins and is mounted to a small PCB. The PCB mounted version is nice because it includes a surface mounted 10K Ohm pull up resistor for the signal line. Here are the pin outs for both versions:

Comparison of three pin DHT11 vs four pin DHT11

HOW TO SET UP THE DHT11 ON AN ARDUINO
Wiring the DHT11 to the Arduino is really easy, but the connections are different depending on which type you have.

CONNECTING A THREE PIN DHT11:
Arduino DHT11 Tutorial - 3 Pin DHT11 Wiring Diagram

CONNECTING A FOUR PIN DHT11:
Arduino DHT11 Tutorial - 4 Pin DHT11 Wiring Diagram

R1: 10K Ohm pull up resistor
DISPLAY HUMIDITY AND TEMPERATURE ON THE SERIAL MONITOR
Before you can use the DHT11 on the Arduino, you’ll need to install the DHTLib library. It has all the functions needed to get the humidity and temperature readings from the sensor. It’s easy to install, just download the DHTLib.zip file below and open up the Arduino IDE. Then go to Sketch>Include Library>Add .ZIP Library and select the DHTLib.zip file.

Circuit Basics ZIP Icon DHTLib

After it’s installed, upload this example program to the Arduino and open the serial monitor:


You should see the humidity and temperature readings displayed at one second intervals.

If you don’t want to use pin 7 for the data signal, you can change the pin number in line 5 where it says #define DHT11_PIN 7.

DISPLAY HUMIDITY AND TEMPERATURE ON AN LCD
A nice way to display the humidity and temperature readings is on a 16X2 LCD. To do this, first follow our tutorial on How to Set Up an LCD Display on an Arduino, then upload this code to the Arduino:


USING THE DATA IN OTHER PROGRAMS
What if you don’t want to output the actual humidity and temperature readings, but need them to calculate or control other things? The code below is the bare minimum needed to initialize the sensor. You can add this to existing programs and use DHT.humidity and DHT.temperature as variables in any function.


To see an example of using the DHT11 sensor outputs as variables in other functions, check out our article How to Set Up an Ultrasonic Range Finder on an Arduino, where we use the DHT.humidity and DHT.temperature  variables in a formula that improves the accuracy of an ultrasonic range finder.

You can watch how to set up the DHT11 and see how it works in this video:


If you have any questions about how to set up the DHT11 humidity and temperature sensor on your Arduino, just leave a comment below and I will try to answer it… And if you like our tutorials, please subscribe! Also, feel free to share this if you know anyone else that might find it helpful!

JLCPCB - Only $2 for PCB Prototype (Any Color)
Great Quality Approved by 600,000+ Customers, 10,000+ PCB Orders Per Day. 
Sign Up & Get at Least Two $5 Coupons Now: https://jlcpcb.com/quote