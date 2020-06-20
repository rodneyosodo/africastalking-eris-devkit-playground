# 74HC595 Shift Register

#### tags: `eris-devkit`

## INTRODUCTION

Have you ever found yourself controlling lots of LED’s? Or just needed more I/O pins to control buttons, sensors, servos all at once? Well, you could wire up few sensors to eris-dev-kit pins, but you would rapidly start to run out of pins on your eris-dev-kit.

The solution for this is to use a Shift Register. Shift registers are often used for the purpose of saving pins on the microcontroller, as every microcontroller has a limited number of GPIO.

## BACKGROUND INFO

A shift register allows you to expand the number of I/O pins you can use from your eris-dev-kit or any micro-controller. And the 74HC595 shift register is one of the most famous among all.

The 74HC595 controls eight separate output pins, using only three input pins. And if you need more than 8 additional I/O lines, you can easily daisychain as many shift registers as you like and create tons of I/O lines. All of this is accomplished by what is known as bit-shifting.

Shift registers come in two basic types

* SIPO \(Serial-In-Parallel-Out\) eg 74HC595 - used for controlling large number of outputs
* PISO \(Parallel-In-Serial-Out\) eg 74HC165 - used for collecting large number of inputs

### How the 74HC595 works

The 595 has two registers \(which can be thought of as “memory containers”\), each with just 8 bits of data. The first one is called the Shift Register. The Shift Register lies deep within the IC circuits, quietly accepting input.

Whenever we apply a clock pulse to a 595, two things happen:

The bits in the Shift Register move one step to the left. For example, Bit 7 accepts the value that was previously in bit 6, bit 6 gets the value of bit 5 etc.

Bit 0 in the Shift Register accepts the current value on DATA pin. At the rising edge of the pulse, if the data pin is high, then a 1 gets pushed into the shift register. Otherwise, it is a 0.

On enabling the Latch pin, the contents of Shift Register are copied into the second register, called the Storage/Latch Register. Each bit of the Storage Register is connected to one of the output pins of the IC, so in general, when the value in the Storage Register changes, so do the outputs.

You can understand this better with the illustration shown below. ![](https://i.imgur.com/vswYJLR.png)

### Pinout

![](https://i.imgur.com/8YnBZUH.png)

Let’s take a look at its Pinout. Notice that two pins have a line over their name; this means they operate in “negative logic”.

* **GND** should be connected to the ground of eris-dev-kit.
* **VCC** is the power supply for 74HC595 shift register which we connect the 5V pin on the eris-dev-kit.
* **SER \(Serial Input\)** is used to feed data into the shift register a bit at a time.
* **SRCLK \(Shift Register Clock\)** is the clock for the shift register. The 595 is clock-driven on the rising edge. This means that in order to shift bits into the shift register, the clock must be HIGH. And bits are transferred in on the rising edge of the clock.
* **RCLK \(Register Clock / Latch\)** When driven HIGH, the contents of Shift Register are copied into the Storage/Latch Register; which ultimately shows up at the output. So the latch pin can be seen as like the final step in the process to seeing our results at the output, which in this case are LEDs.
* **SRCLR \(Shift Register Clear\)** pin allows us to reset the entire Shift Register, making all its bits 0, at once. This is a negative logic pin, so to perform this reset; we need to set the SRCLR pin LOW. When no reset is required, this pin should be HIGH.
* **OE \(Output Enable\)** is negative logic too: When the voltage on it is HIGH, the output pins are disabled/set to high impedance state and don’t allow current to flow. When OE gets low voltage, the output pins work normally.
* **QA–QH \(Output Enable\)** are the output pins and should be connected to some type of output like LEDs, 7 Segments etc.
* **QH’** Pin outputs bit 7 of the ShiftRegister. It is there so that we may daisychain 595s: if you connect this QH’ to the SER pin of another 595, and give both ICs the same clock signal, they will behave like a single IC with 16 outputs. You can daisychain as many as you like, if you have enough power for all of them.

## PROCEDURE

### How to setup DHT11

Start by placing the shift register on to your breadboard, ensuring each side of the IC is on a separate side of the breadboard. With the little U-shaped notch facing upwards, the pins are 1-8 down the left hand side from top to bottom and 16 – 9 down the right hand side from top to bottom as can be seen in the illustration below.

To start with let’s connect pins 16 \(VCC\) and 10 \(SRCLR\) to the 5v pin on the eris-dev-kit and connect pins 8 \(GND\) and 13 \(OE\) to the Gnd pin on the eris-dev-kit. This should keep the IC into the normal working mode.

Next we need to connect the three pins that we will control the shift register with:

* Pin 11 \(SRCLK\) of the shift register to pin PB1 on the eris-dev-kit
* Pin 12 \(RCLK\) of the shift register to pin PA7 on the eris-dev-kit
* Pin 14 \(SER\) of the shift register to pin PA6 on the eris-dev-kit

  Now, we just have to connect up all of the output pins to our LEDs, ensuring that a 220Ω resistor is placed before the LEDs as to reduce the current and that the cathodes of the LEDs go back to ground.

When placing the LEDs be sure that they are connected in order, so that QA is wired to the first LED, and QH is wired to the last LED, as otherwise our code is not going to light up the LEDs in the correct order!

### Programming

```cpp
int latchPin = 5; // Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6; // Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4; // Data pin of 74HC595 is connected to Digital pin 4
byte leds = 0; // Variable to hold the pattern of which LEDs are currently turned on or off


void setup() {
    // Set all the pins of 74HC595 as OUTPUT
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);  
    pinMode(clockPin, OUTPUT);
}

void loop() {
    leds = 0; // Initially turns all the LEDs off, by giving the variable 'leds' the value 0
    updateShiftRegister();
    delay(500);
    for (int i = 0; i < 8; i++)    // Turn all the LEDs ON one by one.
    {
        bitSet(leds, i); // Set the bit that controls that LED in the variable 'leds'
        updateShiftRegister();
        delay(500);
    }
}

/*
 * updateShiftRegister() - This function sets the latchPin to low, then calls the eris-dev-kit function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
 */
void updateShiftRegister(){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, leds);
    digitalWrite(latchPin, HIGH);
}
```

If you wanted to turn one of the LEDs off rather than on, you would call a similar eris-dev-kit function bitClear\(\) on the ‘leds’ variable. This will set that particular bit of ‘leds’ to be 0 and you would then just need to call updateShiftRegister\(\) to update the actual LEDs.

## APPLICATIONS

* Nintendo Controller The main controller of the Nintendo Entertainment System needed to get all button presses serially, and it used a shift register to accomplish that task.
* 15 Channel relay board

