## Introduction
The DHT11 humidity and temperature sensor makes it easy to add humidity and temperature data for your IoT solution. It does its job but it's not the best. 

## Background Info
We will first look at humidity the explain how the DHT11 works.

> Here are the ranges and accuracy of the DHT11:
> * Humidity Range: 20-90% RH
> * Humidity Accuracy: ±5% RH
> * Temperature Range: 0-50 °C
> * Temperature Accuracy: ±2% °C
> * Operating Voltage: 3V to 5.5V

#### WHAT IS RELATIVE HUMIDITY?
Relative humidity is the amount of water vapour currently in the atmosphere compared to the saturation point of water vapour in the atmosphere. At saturation point water vapour starts to condense and form dew. At saturation temp, cold air and hold less water vapour while hot air can hold more water vapour. 

Relative humidity is expressed as a percentage. At 100% condensation occurs and at 0% the air is completely dry.


#### HOW THE DHT11 WORK
The DHT11 detects water vapor by measuring the electrical resistance between two electrodes. The humidity sensing component is a moisture holding substrate with electrodes applied to the surface. When water vapor is absorbed by the substrate, ions are released by the substrate which increases the conductivity between the electrodes. The change in resistance between the two electrodes is proportional to the relative humidity. Higher relative humidity decreases the resistance between the electrodes, while lower relative humidity increases the resistance between the electrodes.

The DHT11 measures temperature with a thermistor built into the unit. An IC on the unit stores the calibration coefficients, and controls the data signal transmission between the DHT11 and microcontroller

The DHT11 uses one signal wire to transmit data and power comes from the 5V and ground pin.

#### HOW TO SET UP THE DHT11
Wiring the DHT11 to the Arduino is really easy. Connect the signal pin to a GPIO pin and the 5V to 5V pin on the dev kit and Ground to the ground pin.
Connect the module to any 3 pin brackets e.g to PB0, PB1, PA6 brackets

#### Programming
Before you can use the DHT11, you’ll need to install the DHTLib library. It is will written and has good examples. It is easy to install, Go to tools the manage library then search `DHT sensor library`
![](https://i.imgur.com/r6rYvCK.png)

After it’s installed, upload this example program to the Arduino and open the serial monitor:

```cpp

#include <DHT.h>

int dhtPin = PA6;
float humidity, temperature, heatIndex;

#define DHTTYPE DHT11

DHT dht(dhtPin, DHTTYPE);

void setup(){
    Serial.begin(9600);
    Serial.println("Starting DHT11");
    dht.begin();
}

void loop(){
    // Wait for a few seconds to measure
    delay(1000);
    // Reading takes about 250 milliseconds
    humidity = dht.readHumidity();

    // Read temperatire in Celsius(default)
    temperature = dht.readTemperature();

    // Check if reads failed and exit
     if (humidity ==   NULL   || temperature ==   NULL){
        Serial.println("Failed to read from DHT");
         return;
    }
    Serial.print("\nHumidity: ");
    Serial.print(humidity);
    Serial.print("%\tTemperature: ");
    Serial.print(temperature);
    Serial.print("°C");
}

```
You should see the humidity and temperature readings displayed at one second intervals.

If you would like to computer the heat index the formula is
```cpp
float H = dht.readHumidity(); // read humitity as %f
float C = dht.readTemperature(); // read temperature as Celsius (default)
float F = dht.readTemperature(true); // read temperature as Fahrenheit (if true)

// Compute heat index in Celsius (isFahreheit = false)

float hic = dht.computeHeatIndex(C, H, false);

```

```cpp
float H = dht.readHumidity(); // read humitity as %f
float C = dht.readTemperature(); // read temperature as Celsius (default)
float F = dht.readTemperature(true); // read temperature as Fahrenheit (if true)

// calculations and conversions section
float HiF = dht.computeHeatIndex(F, H); // read and compute heat index in Fahrenheit (the default)
float DewPoint = (C - (100 - H) / 5); // dewpoint calculation using Celsius value
float DP = (DewPoint * 9 / 5) + 32; // converts dewPoint calculation to fahrenheit

```

## Applications
- remote weather stations
- home environmental control systems
- farm or garden monitoring systems.
