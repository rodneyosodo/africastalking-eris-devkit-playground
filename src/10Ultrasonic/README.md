# ULTRASONIC
###### tags: `eris-devkit`

## INTRODUCTION
Give your next project bat-powers with a HC-SR04 Ultrasonic Distance Sensor that can report the range of objects up to 4 metres away.

## BACKGROUND INFO
> Here are complete specifications:
> * Operating Voltage	DC 5V
> * Operating Current	15mA
> * Operating Frequency	40KHz
> * Max Range	4m
> * Min Range	2cm
> * Ranging Accuracy	3mm
> * Measuring Angle	15 degree
> * Trigger Input Signal	10µS TTL pulse
> * Dimension	45 x 20 x 15mm
> * HC-SR04 Ultrasonic Sensor Pinout

#### What is Ultrasound?

Ultrasound is a high-pitched sound wave with frequencies higher than the audible limit of human hearing.
Ultrasonic Frequency Range Spectrum

Human ears can hear sound waves that vibrate in the range from about 20 times a second (a deep rumbling noise) to about 20,000 times a second (a high-pitched whistling). However, ultrasound has a frequency of over 20,000 Hz and is, therefore, inaudible to humans.

#### HC-SR04 Hardware Overview

At its core, the HC-SR04 Ultrasonic distance sensor consists of two ultrasonic transducers. The one acts as a transmitter, which converts an electrical signal into 40 kHz ultrasonic sound pulses. The receiver listens for the transmitted pulses. If it receives them, it produces an output pulse whose width can be used to determine the distance the pulse traveled. As simple as pie!

#### How it works
It all starts when a pulse of at least 10 microseconds in duration is applied to the Trigger pin. In response to that, the sensor transmits a sonic burst of eight pulses at 40 kHz. This 8-pulse pattern makes the “ultrasonic signature” from the device unique, allowing the receiver to differentiate the transmitted pattern from the ambient ultrasonic noise.

The eight ultrasonic pulses travel through the air away from the transmitter. Meanwhile, the Echo pin goes HIGH to start forming the beginning of the echo-back signal.

If those pulses are not reflected back, then the echo signal will timeout after 38 milliseconds and return low. 

If those pulses are reflected back, the Echo pin goes LOW as soon as the signal is received. This produces a pulse whose width varies between 150 µS to 25 mS, depending upon the time it took for the signal to be received.

The width of the received pulse is then used to calculate the distance to the reflected object. This can be worked out using a simple distance-speed-time equation.
```cpp
Distance = Speed x Time
Distance = (0.034 cm/µs x 500 µs) / 2
Distance = 8.5 cm
```


#### Pinout 
Generic

![](https://i.imgur.com/dkHy6lc.png)

How it looks

![](https://i.imgur.com/3uen6hu.jpg)


* **VCC** is the power supply for HC-SR04 Ultrasonic distance sensor which we connect the 5V pin on the eris-dev-kit.
* **Trigger pin**  is used to trigger the ultrasonic sound pulses.
* **Echo pin** produces a pulse when the reflected signal is received. The length of the pulse is proportional to the time it took for the transmitted signal to be detected.

## PROCEDURE
#### How to setup DHT11
Start by placing the sensor on to your breadboard. Connect VCC pin to the 5V pin on the eris-dev-kit and connect GND pin to the Ground pin on the eris-dev-kit.

![](https://i.imgur.com/cyzx1Sy.png)

#### Programming
Before doing the hard process, we will use the NewPing library. It is well written and has good examples. To install this library, go to tools, manage library then search `New Ping`.



```clike
// This uses Serial Monitor to display Range Finder distance readings
// This uses Serial Monitor to display Range Finder distance readings

// Include NewPing Library
#include   <NewPing.h>

// Hook up HC-SR04 with Trig to eris-dev-kit Pin PB10, Echo to eris-dev-kit pin PB11
#define TRIGGER_PIN PB10
#define ECHO_PIN PB11

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE   400 

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

float duration, distance;

void setup(){
    Serial.begin(9600);
}

void loop(){
    // Send ping, get distance in cm
    distance = sonar.ping_cm();
    // duration = sonar.ping();
    // distance = (duration / 2) * 0.0343;

    // Send results to Serial Monitor
    Serial.print("Distance = ");
    if (distance >= 400 || distance <= 2){
        Serial.println("Out of range");
    }
    else{
        Serial.print(distance);
        Serial.println(" cm");
    }
    delay(500);
}

```
If you want to bring back the decimal point values you can use NewPing in duration mode instead of the distance mode. You need to replace this line

```cpp
// Send ping, get distance in cm
distance = sonar.ping_cm();
```
with below lines
```cpp
duration = sonar.ping();
distance = (duration / 2) * 0.0343;
```
To improve the accuracy we will use the ping median function. It takes many duration measurements instead of just one, throws away any invalid readings and then averages the remaining ones. By default it takes 5 readings but you can specify as many as you wish.
```cpp
int iterations = 5;
duration = sonar.ping_median(iterations);
```

## APPLICATIONS
- Remote controlled cars