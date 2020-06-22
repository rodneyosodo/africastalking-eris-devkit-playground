---
description: Working with the DHT11 humidity and temperature sensor
---

# DHT11

#### tags: `eris-devkit`

## INTRODUCTION

The DHT11 humidity and temperature sensor makes it easy to add humidity and temperature data for your IoT solution. It does it's job well although it's not the best. 

## BACKGROUND INFO

First , we will look at humidity the explain how the DHT11 works.

> Here are the ranges and accuracy of the DHT11:
>
> * Humidity Range: 20-90% RH
> * Humidity Accuracy: ±5% RH
> * Temperature Range: 0-50 °C
> * Temperature Accuracy: ±2% °C
> * Operating Voltage: 3V to 5.5V

### What is relative humidity?

Relative humidity is the amount of water vapor compared to the saturation point of water vapor in the atmosphere. At saturation point, water vapor starts to condense to form dew. At saturation temp, less water vapor is held by cold air compared to hot air which can hold more water vapor.

Relative humidity is expressed as a percentage. At 100% condensation occurs while at 0% air is  dehydrated.

### How the DHT11 works

The DHT11 detects water vapor by measuring the electrical resistance between two electrodes. The humidity sensing component is a moisture holding substrate with electrodes applied to the surface. When the substrate absorbs water vapor, it releases ions that in turn increase the conductivity between the electrodes. Change in resistance between the electrodes is proportional to relative humidity. Higher relative humidity decreases the resistance between the electrodes, while lower relative humidity increases the resistance between the electrodes.

The DHT11 measures temperature with a thermistor built into the unit. An IC on the unit stores the calibration coefficients and controls the data signal transmission between the DHT11 and micro controller.

### Pin out

Generic dht11

![](https://i.imgur.com/mAcYXTN.png)

What it looks like

![](https://i.imgur.com/UgNrGAM.jpg)

* **GND** is a common ground for both the dht11 and micro controller.
* **5 V** is a positive voltage that powers the dht11.
* **Control** transmit data

## PROCEDURE

### How to setup DHT11

Wiring the DHT11 to the eris dev kit is easy. Connect the Red wire or VCC pin to the 5V on the dev kit and Black wire or GND pin to ground. Finally, connect the Orange wire or Data pin to pin PA7. Connect the module to any 3 pin brackets e.g., to PB0, PB1, PA6 brackets.

![](https://i.imgur.com/lCILAaW.png)

### Programming

Before you can use the DHT11, you'll need to install the DHTLib library. It is well written and has good examples. To install the library, go to tools, manage the library, and then search `DHT sensor library.` 

![](https://i.imgur.com/r6rYvCK.png)

After installation, upload this example program to the Arduino and open the serial monitor:

```cpp
#include <DHT.h>

int dhtPin = PA6; // Pin where the data pin is connected
float humidity, temperature, heatIndex;

#define DHTTYPE DHT11 // define dht module

DHT dht(dhtPin, DHTTYPE); // create dht object initialized with the signal pin and the type of dht module

void setup(){
    Serial.begin(9600);
    Serial.println("Starting DHT11");
    dht.begin(); // starts the dht
}

void loop(){
    // Wait for a few seconds to measure
    delay(1000);
    // Reading takes about 250 milliseconds
    humidity = dht.readHumidity();

    // Read temperatire in Celsius(default)
    temperature = dht.readTemperature();

    // Check if reads failed and exit
     if (humidity == NULL || temperature == NULL){
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

You should see the humidity and temperature readings displayed at one-second intervals. If you would like to computer the heat index the formula is

```cpp
float H = dht.readHumidity(); // read humitity as %f
float C = dht.readTemperature(); // read temperature as Celsius (default)
float F = dht.readTemperature(true); // read temperature as Fahrenheit (if true)

// Compute heat index in Celsius (isFahreheit = false)

float hic = dht.computeHeatIndex(C, H, false);

```

If you would like to computer the dew point the formula is

```cpp
float H = dht.readHumidity(); // read humitity as %f
float C = dht.readTemperature(); // read temperature as Celsius (default)
float F = dht.readTemperature(true); // read temperature as Fahrenheit (if true)

// calculations and conversions section
float HiF = dht.computeHeatIndex(F, H); // read and compute heat index in Fahrenheit (the default)
float DewPoint = (C - (100 - H) / 5); // dewpoint calculation using Celsius value

```

## APPLICATIONS

* remote weather stations
* home environmental control systems
* farm or garden monitoring systems.

