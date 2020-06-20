# LDR
###### tags: `eris-devkit`

## INTRODUCTION
We all want our home appliances to be controlled automatically based on some conditions, which is called Home Automation. We will learn how to use light intensity to control devices. For this, we need a light sensor to detect the light condition and some circuitry to control the light sensor. We will use the Light Dependent Resistor.


## BACKGROUND INFO
LDR is a Light Dependent Resistor. LDRs are made from semiconductor materials which enable them to have their light-sensitive properties. 

#### What is a Light Dependent Resistor?
A photoresistor, LDR for a light-dependent resistor, is a passive component that decreases resistance with respect to receiving light on the component's sensitive surface. The resistance of a photoresistor decreases with an increase in incident light intensity; in other words, it exhibits photoconductivity.

#### How the LDR works?
When light falls on the surface of the LDR the conductance of the element increases, the resistance of the LDR decreases. This property of the LDR is achieved because it is a property of the semiconductor material used on the surface.

#### Pinout

Generic

![](https://i.imgur.com/ExTQzjt.png)

How it looks like

![](https://i.imgur.com/46eWuFM.jpg)


* **GND** is a common ground for both the motor and logic.
* **5V** is a positive voltage that powers the servo.
* **Signal** transmit data

## PROCEDURE
#### How to setup DHT11

![](https://i.imgur.com/gGia4LK.png)

Wiring the LDR to the eris-dev-kit is easy.
Connect the Red wire or Vcc pin to the 5V on the dev kit and Black wire or GND pin to ground. Finally connect the Orange wire or Data pin to the PWM enabled pin PA7.
Connect the module to any 3 pin brackets e.g to PB0, PB1, PA6 brackets

- Connect the 3.3v output of the eris-dev-kit to the positive rail of the breadboard
- Connect the ground to the negative rail of the breadboard
- Place the LDR on the breadboard
- Attach the 10K resistor to one of the legs of the LDR
- Connect the PA6 pin of the eris-dev-kit to the same column where the LDR and resistor is connected (Since the LDR gives out an analog voltage, it is connected to the analog input pin on the eris-dev-kit. With an in-built ADC (Analog to Digital Converter), it converts the analog voltage from 0-5V into a digital value in the range of 0-4095).
-  Now connect the other end of the 10K resistor to the negative rail and the the second (free) leg of the LDR to the positive rail


#### Programming
The analog pin senses the voltage and gives some analog value to eris-dev-kit. The analog value changes according to the resistance of LDR. So, as the light falls on the LDR, the resistance of it get decreased and hence the voltage value increase.

```cpp
const int ldrPin = PA6; // declaring the analog input pint as PA6

void setup() {
    Serial.begin(9600);
    pinMode(ldrPin, INPUT); // Setting the oin to be an input pin
}
void loop() {
    int ldrStatus =   analogRead(ldrPin);// Reading the values from 0 - 4095
    Serial.print("Value: "); //Printing the values to serial
    Serial.println(ldrStatus);
    delay(100); // waiting for 100ms
}

```

Controlling the inbuilt led with the LDR

```cpp
// We define the Pins for LED and LDR.
const int ledPin = PC13;
const int ldrPin = PA6;

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(ldrPin, INPUT);
}

void loop() {
    int ldrStatus =   analogRead(ldrPin);
    if (ldrStatus >= 2000) {
        digitalWrite(ledPin, HIGH);
        Serial.print("Its DARK, LED is on : ");
        Serial.println(ldrStatus);
    } else {
        digitalWrite(ledPin, LOW);
        Serial.print("Its BRIGHT, LED is off : ");
        Serial.println(ldrStatus);
    }
    delay(100);
}

```
As per the eris-dev-kit code, if the analog value falls below 2000 we consider it as dark and the light turns ON. If the value comes above 2000 we consider it as bright and the light turns OFF.

![](https://i.imgur.com/5EJQfHl.jpg)

## APPLICATIONS
- Home automation
- Street lamps
- Alarm devices
- Farm or garden monitoring systems
