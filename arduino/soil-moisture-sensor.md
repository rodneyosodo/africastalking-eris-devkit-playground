# Soil Moisture Sensor

#### tags: `eris-devkit`

## INTRODUCTION

The soil moisture sensor gives us the relative soil moisture content compared to when the soil is saturated with water. It gives us an analog signal of current soil moisture!

## BACKGROUND INFO

This soil hygrometer measures the amount of water content inside the soil based on electrical resistance and gives the moisture level as output. The sensor is equipped with both analog and digital output so that it can be used in both analog and digital modes.

Specifications

* Input Voltage    3.3 – 5 V
* Output Voltage 0 – 4.2 V
* Input Current    35 mA
* Output Signal    Both Analog and Digital

### Soil moisture

Soil moisture is essentially the content of water present in the soil. Relative soil moisture is the amount of water currently in the soil compared to when the soil is completely logged with water.

### How the soil hygrometer works

The soil moisture sensor consists of two conducting plates which function as a probe. This sensor is used to measure the volumetric content of water.

It can measure the moisture content in the soil based on the change in resistance between the two conducting plates. When there is more water, the soil will conduct more electricity, which means that there will be less resistance. Therefore, the moisture level will be higher. Similarly, dry soil conducts less electricity. When there is less water, the soil will conduct less electricity, which means that there will be more resistance. Therefore, the moisture level will be lower.

### Pin out

Generic

![](https://i.imgur.com/YCLNE50.png)

What it looks like

![](https://i.imgur.com/zVX0Jl2.jpg)

* **GND** is a common ground for both the soil moisture sensor and micro controller.
* **5V** is a positive voltage that powers the soil moisture sensor.
* **Control** transmit data

## PROCEDURE

### Calibration

For the soil moisture sensor to give accurate readings, it is recommended that you first calibrate it for the particular type of soil that you plan to monitor.

Different types of soil can affect the sensor, so your sensor may be more or less sensitive depending on the type of soil you use.

Before you start storing data or triggering events, you should see what readings you are getting from your sensor.

### How to setup Soil hygrometer

Wiring the module to the eris-dev-kit is easy. Connect the Red wire or VCC pin to the 5V on the dev kit and Black wire or GND pin to ground. Finally, connect the Orange wire or Data pin to pin PA7. Connect the module to any 3 pin brackets e.g., to PB0, PB1, PA6 brackets

![](https://i.imgur.com/mpteb0L.png)

### Programming

To make it operate in Analog Mode, we need to connect the analog output of the sensor to the ADC input of the eris-dev-kit. While monitoring the sensor, we will get ADC values from 0-4095. The moisture content will be measured in percentage, so we have to map the values from 0 -100, which will be displayed on Serial Monitor.

```cpp
const int soilMoisturePin = PA6; // Pin where the data pin is connected

void setup() {
    Serial.begin(9600); // initializing the serial port of eris-dev-kit to display information in the serial monitor
    pinMode(soilMoisturePin, INPUT); // Setting the pin as input
}

void loop() {
    int soilMoistureStatus = analogRead(soilMoisturePin); // Reading analog values from the pin
    Serial.print("Value: ");
    Serial.println(soilMoistureStatus);
    delay(100);
}
```

If you want to get the percentage

```cpp
void loop() {
    int analogValue = analogRead(soilMoisturePin);
    soilMoistureValue = map(analogValue, 0, 4095, 0, 100); //Map value : 0 will be 0 and 4095 will be 100
    Serial.print("Soil humidity: ");
    Serial.print(soilMoistureValue);
    Serial.println("%");
    delay(1000);
}
```

![Measuring soil moisture content](https://i.imgur.com/TxK1X6M.jpg)

![Measuring soil moisture content on water](https://i.imgur.com/vZqCwHZ.jpg)

## APPLICATIONS

* Agriculture Department
* Landscape Irrigation
* Gardening purpose

