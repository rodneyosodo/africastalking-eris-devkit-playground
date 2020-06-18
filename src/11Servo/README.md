## INTRODUCTION
Want to add motion to your next Arduino project without building a motor controller? Then servo motors might be the solid launching point for you. With servo motors you can precisely control the positioning of these motors. Instructing them where to point, and they’ll do it for you.

## BACKGROUND INFORMATION

#### What is Servo?
Servo is a general term for a closed loop control system.

A closed loop system uses the feedback signal to adjust the speed and direction of the motor to achieve the desired result.

The output shaft drives a servo arm and is also connected to a potentiometer (pot).

The potentiometer provides position feedback to the servo control unit where the current position of the motor is compared to the target position.

According to the error, the control unit corrects the actual position of the motor so that it matches the target position.

#### How Servo Motors Work?
You can control the servo motor by sending a series of pulses to the signal line. A conventional analog servo motor expects to receive a pulse roughly every 20 milliseconds (i.e. signal should be 50Hz).

The length of the pulse determines the position of the servo motor.

If the pulse is high for 1ms, then the servo angle will be zero.
If the pulse is high for 1.5ms, then the servo will be at its center position.
If the pulse is high for 2ms, then the servo will at 180 degrees.
Pulses ranging between 1ms and 2ms will move the servo shaft through the full 180 degrees of its travel.

#### Pinout
![](https://i.imgur.com/4LFDthm.png)

* **GND** (black or brown wire) is a common ground for both the motor and logic.
* **5V** (red wire) is a positive voltage that powers the servo.
* **Control** (orange or yellow wire)is input for the control system.


For example let’s use SG90 Micro Servo Motor. It runs on 4.8-6VDC (5V Typical) and can rotate approximately 180 degrees (90 in each direction)..

It consumes around 10mA at idle and 100mA to 250mA when moving, so we can power it up through 5-volt output on the dev kit.

If you have a servo that consumes more than 250mA, consider using a separate power supply for your servo.

## PROCEDURE
Before you can use the servo motor, you’ll need to use the Servo library. It is will written.
![](https://i.imgur.com/n7N7OPA.png)

#### How to setup Servo motor
Connect the Red wire to the 5V on the dev kit (or DC jack) and Black/Brown wire to ground. Finally connect the Orange/Yellow wire to the PWM enabled pin PA7.

#### Sweep

```cpp
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup() {
    myservo.attach(PA7);  // attaches the servo on pin 9 to the servo object
}

void loop() {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
        myservo.write(pos);// tell servo to go to position in 'pos'
        delay(15); // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);
        delay(15);
    }
}

```
#### Controlling servo with a potentiometer
The code to make the servo follow the knob’s position is simpler than to make it sweep.

##### Wiring
As the wiring diagram shows you’ll need a potentiometer, any value from 10k up will be OK. Connect one end of the pot to ground, the other end to the Arduino 5V and the wiper to analog input PA7.

```cpp
#include <Servo.h>

int potPin = PA7;
int servoPin = PA6;
Servo servo;

void setup() {
    servo.attach(servoPin);
}

void loop() {
    int reading = analogRead(potPin); // reading the value from the analog pin PA7 and gives us values between 0 and 4095
    int angle = map(reading, 0, 4095, 0, 180); // re maps the number range 0-4095 to 0-180 to represent angle in degrees
    servo.write(angle); // command that tells the servo to update its position to the angle selected by the potentiometer.
}
```

## Applications
- turning the front wheels on an RC model for steering
- pivoting a sensor to look around on a robotic vehicle.
