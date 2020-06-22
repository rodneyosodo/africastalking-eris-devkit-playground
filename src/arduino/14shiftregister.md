# README

How 74HC595 Shift Register Works & Interface it with Arduino Project Working and Interfacing 74HC595 Shift Register with Arduino

Have you ever found yourself wanting to control lots of LED’s? Or just needed more I/O pins to control buttons, sensors, servos all at once? Well, you could wire up few sensors to Arduino pins, but you would rapidly start to run out of pins on your Arduino.

The solution for this is to use a ‘Shift Register’. A shift register allows you to expand the number of I/O pins you can use from your Arduino \(or any microcontroller for that matter\). And the 74HC595 shift register \(nicknamed ‘595’\) is one of the most famous among all.

The 595 essentially controls eight separate output pins, using only three input pins. And if you need more than 8 additional I/O lines, you can easily daisychain as many shift registers as you like and create tons of I/O lines. All of this is accomplished by what is known as bit-shifting. If you want to know more about bit-shifting, this resource from Wikipedia is invaluable. When to use Shift Register?

Shift registers are often used for the purpose of saving pins on the microcontroller, as every microcontroller has a limited number of I/O pins \(GPIO\).

If your project needs to control 16 individual LEDs, that would normally require 16 pins of an Arduino. In the event that you don’t have 16 I/O pins available, this is where the shift register comes in handy. With two shift registers connected in series, we can accomplish the task of controlling the 16 LEDs with using only 3 I/O pins. And not just this; you can save even more pins the more shift registers you have chained together.

A real world example that uses shift register is the ‘Original Nintendo Controller’. The main controller of the Nintendo Entertainment System needed to get all button presses serially, and it used a shift register to accomplish that task. Nintendo-Entertainment-System-NES-Controller-FL Photo credit: Wikipedia SIPO Vs PISO Shift Registers

Shift registers come in two basic types, either SIPO \(Serial-In-Parallel-Out\) or PISO \(Parallel-In-Serial-Out\). The popular SIPO chip is 74HC595, and the PISO chip is 74HC165.

The first type, SIPO, is useful for controlling a large number of outputs, like LEDs. While the latter type, PISO, is good for gathering a large number of inputs, like buttons; like the one used in Original Nintendo Controller as discussed above. How 74HC595 Shift Register works?

The 595 has two registers \(which can be thought of as “memory containers”\), each with just 8 bits of data. The first one is called the Shift Register. The Shift Register lies deep within the IC circuits, quietly accepting input.

Whenever we apply a clock pulse to a 595, two things happen:

```text
The bits in the Shift Register move one step to the left. For example, Bit 7 accepts the value that was previously in bit 6, bit 6 gets the value of bit 5 etc.
Bit 0 in the Shift Register accepts the current value on DATA pin. At the rising edge of the pulse, if the data pin is high, then a 1 gets pushed into the shift register. Otherwise, it is a 0.
```

On enabling the Latch pin, the contents of Shift Register are copied into the second register, called the Storage/Latch Register. Each bit of the Storage Register is connected to one of the output pins QA–QH of the IC, so in general, when the value in the Storage Register changes, so do the outputs.

You can understand this better with the illustration shown below. 74HC595 Shift Register Working 74HC595 Shift Register Working 74HC595 Shift Register Pinout

The 595 comes in a variety of makes and models; here we will talk about the ubiquitous Texas Instruments SN74HC595N. If you get a different one, read its datasheet carefully and make note of any differences.

Let’s take a look at its Pinout. Notice that two pins have a line over their name; this means they operate in “negative logic”. You will come to know about that a little later. Pinout 74HC595 Shift Register

GND should be connected to the ground of Arduino.

VCC is the power supply for 74HC595 shift register which we connect the 5V pin on the Arduino.

SER \(Serial Input\) pin is used to feed data into the shift register a bit at a time.

SRCLK \(Shift Register Clock\) is the clock for the shift register. The 595 is clock-driven on the rising edge. This means that in order to shift bits into the shift register, the clock must be HIGH. And bits are transferred in on the rising edge of the clock.

RCLK \(Register Clock / Latch\) is a very important pin. When driven HIGH, the contents of Shift Register are copied into the Storage/Latch Register; which ultimately shows up at the output. So the latch pin can be seen as like the final step in the process to seeing our results at the output, which in this case are LEDs.

SRCLR \(Shift Register Clear\) pin allows us to reset the entire Shift Register, making all its bits 0, at once. This is a negative logic pin, so to perform this reset; we need to set the SRCLR pin LOW. When no reset is required, this pin should be HIGH.

OE \(Output Enable\) is negative logic too: When the voltage on it is HIGH, the output pins are disabled/set to high impedance state and don’t allow current to flow. When OE gets low voltage, the output pins work normally.

QA–QH \(Output Enable\) are the output pins and should be connected to some type of output like LEDs, 7 Segments etc.

QH’ Pin outputs bit 7 of the ShiftRegister. It is there so that we may daisychain 595s: if you connect this QH’ to the SER pin of another 595, and give both ICs the same clock signal, they will behave like a single IC with 16 outputs. Of course, this technique is not limited to two ICs – you can daisychain as many as you like, if you have enough power for all of them. Wiring – Connecting 74HC595 Shift Register to Arduino UNO

Now that we have a basic understanding of how 74HC595 works, we can begin hooking it up to our Arduino!

Start by placing the shift register on to your breadboard, ensuring each side of the IC is on a separate side of the breadboard. With the little U-shaped notch facing upwards, the pins are 1-8 down the left hand side from top to bottom and 16 – 9 down the right hand side from top to bottom as can be seen in the illustration below.

To start with let’s connect pins 16 \(VCC\) and 10 \(SRCLR\) to the 5v pin on the Arduino and connect pins 8 \(GND\) and 13 \(OE\) to the Gnd pin on the Arduino. This should keep the IC into the normal working mode.

Next we need to connect the three pins that we will control the shift register with:

```text
Pin 11 (SRCLK) of the shift register to pin 6 on the Arduino
Pin 12 (RCLK) of the shift register to pin 5 on the Arduino
Pin 14 (SER) of the shift register to pin 4 on the Arduino
```

Now, we just have to connect up all of the output pins to our LEDs, ensuring that a 220Ω resistor is placed before the LEDs as to reduce the current and that the cathodes of the LEDs go back to ground.

When placing the LEDs be sure that they are connected in order, so that QA is wired to the first LED, and QH is wired to the last LED, as otherwise our code is not going to light up the LEDs in the correct order!

When you’re done you should have something that looks similar to the illustration shown below. Arduino Wiring Fritzing Connections with 74HC595 Shift Register Wiring 74HC595 Shift Register to Arduino UNO Arduino Code

Now we’re ready to put some code behind it! Plug your Arduino into your computer and Try the sketch out; and then we will dissect it in some detail.

int latchPin = 5; // Latch pin of 74HC595 is connected to Digital pin 5 int clockPin = 6; // Clock pin of 74HC595 is connected to Digital pin 6 int dataPin = 4; // Data pin of 74HC595 is connected to Digital pin 4

byte leds = 0; // Variable to hold the pattern of which LEDs are currently turned on or off

/\*

* setup\(\) - this function runs once when you turn your Arduino on
* We initialize the serial connection with the computer

  \*/

  void setup\(\) 

  {

  // Set all the pins of 74HC595 as OUTPUT

  pinMode\(latchPin, OUTPUT\);

  pinMode\(dataPin, OUTPUT\);  

  pinMode\(clockPin, OUTPUT\);

  }

/\*

* loop\(\) - this function runs over and over again

  \*/

  void loop\(\) 

  {

  leds = 0;    // Initially turns all the LEDs off, by giving the variable 'leds' the value 0

  updateShiftRegister\(\);

  delay\(500\);

  for \(int i = 0; i &lt; 8; i++\)    // Turn all the LEDs ON one by one.

  {

   bitSet\(leds, i\);        // Set the bit that controls that LED in the variable 'leds'

   updateShiftRegister\(\);

   delay\(500\);

  }

  }

/\*

* updateShiftRegister\(\) - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.

  \*/

  void updateShiftRegister\(\)

  {

  digitalWrite\(latchPin, LOW\);

  shiftOut\(dataPin, clockPin, LSBFIRST, leds\);

  digitalWrite\(latchPin, HIGH\);

  }

Once you upload the code to the Arduino, you should see output appear as follows: 74HC595 Shift Register Sketch Output

If you wanted to turn one of the LEDs off rather than on, you would call a similar Arduino function bitClear\(\) on the ‘leds’ variable. This will set that particular bit of ‘leds’ to be 0 and you would then just need to call updateShiftRegister\(\) to update the actual LEDs. Code Explanation:

The first thing we do is define the 3 control pins viz. latch, clock and data pins of the 74HC595, we are going to connect to the Arduino’s digital pins \#5, \#6 and \#4 respectively.

int latchPin = 5; int clockPin = 6; int dataPin = 4;

Next, a variable called ‘leds’ is defined. This will be used to hold the pattern of which LEDs are currently turned on or off. Data of type ‘byte’ represents numbers using eight bits. Each bit can be either on or off, so this is perfect for keeping track of which of our eight LEDs are on or off.

// Variable to hold the pattern of which LEDs are currently turned on or off byte leds = 0;

In the ‘setup‘ function: we simply initialize the three pins to be digital outputs.

void setup\(\) { pinMode\(latchPin, OUTPUT\); pinMode\(dataPin, OUTPUT\);  
pinMode\(clockPin, OUTPUT\); }

The loop function: initially turns all the LEDs off, by setting all the bits of the variable ‘leds’ to 0. It then calls custom function called updateShiftRegister\(\) that will send the ‘leds’ pattern to the shift register so that all the LEDs turn off. We will deal with how updateShiftRegister\(\) works later.

The program halts for half a second and then begins to count from 0 to 7 using the ‘for’ loop and the variable ‘i’. Each time, it uses the Arduino function bitSet\(\) to set the bit that controls particular LED in the variable ‘leds’. It then also calls updateShiftRegister\(\) so that status of LEDs changes according to what is in the variable ‘leds’.

Then there is a half second delay before ‘i’ is incremented and the next LED is lit.

void loop\(\) { leds = 0; updateShiftRegister\(\); delay\(500\); for \(int i = 0; i &lt; 8; i++\) { bitSet\(leds, i\); updateShiftRegister\(\); delay\(500\); } }

The function updateShiftRegister\(\) , first of all sets the latch Pin to LOW, and then calls the Arduino function shiftOut\(\) before putting the latch Pin HIGH again.

Thankfully Arduino provide a helper function specifically for shift registers called shiftOut\(\) , which allows us to simply shift the bits in one call. You can explore more about the library on Arduino’s official website.

The shiftOut\(\) function takes four parameters; the first two are the pins to use for Data and Clock respectively. The third parameter specifies which end of the data you want to start at. We are going to start with the right most bit, which is referred to as the ‘Least Significant Bit’ \(LSB\).

The last parameter is the actual data to be shifted into the shift register, which in this case is ‘leds’.

void updateShiftRegister\(\) { digitalWrite\(latchPin, LOW\); shiftOut\(dataPin, clockPin, LSBFIRST, leds\); digitalWrite\(latchPin, HIGH\); }

Controlling Brightness Using PWM

As a supplement, here’s another project, based on the same setup but with a little difference, where we manipulate another control pin on the IC i.e. OE – and by doing that, we can control the brightness of the output LEDs!

We have already learnt that OE \(Output Enable\) pin acts as a switch. When this pin is set to HIGH, the output pins are disabled \(it works with negative logic, remember?\). And when OE gets low voltage, the output pins work normally.

In our previous example, we had connected this pin permanently to Ground, enabling the outputs all the time. If we connect this pin to any of the Arduino’s digital pins and programmed to toggle its state, we can get something like illustrated below. 74HC595 Shift Register Output Enable Toggle Output

But, instead of doing that, we can use this pin along with the analogWrite\(\) function, to control the brightness of the LEDs using PWM. So, we can get result like the one shown below. 74HC595 Shift Register Output Enable PWM Output

Actually the HIGH parts of the PWM signal will still cause the OE pin to temporarily disable all the IC outputs. But, this will happen, of course, faster than our eyes can perceive directly, but we will definitely sense a variation in the overall brightness.

To do this, all you need to do, is to change the connection to pin 13 of the 74HC595. So that instead of connecting it to Ground, you connect it to pin 3 of the Arduino. Arduino PWM Brightness Control Wiring Fritzing Connections with 74HC595 Shift Register PWM Brightness Control – Wiring 74HC595 Shift Register with Arduino UNO

The below sketch will once all the LEDs have been lit gradually fade them back to off.

int latchPin = 5; // Latch pin of 74HC595 is connected to Digital pin 5 int clockPin = 6; // Clock pin of 74HC595 is connected to Digital pin 6 int dataPin = 4; // Data pin of 74HC595 is connected to Digital pin 4 int outputEnablePin = 3; // OE pin of 74HC595 is connected to PWM pin 3

byte leds = 0; // Variable to hold the pattern of which LEDs are currently turned on or off

/\*

* setup\(\) - this function runs once when you turn your Arduino on
* We initialize the serial connection with the computer

  \*/

  void setup\(\) 

  {

  // Set all the pins of 74HC595 as OUTPUT

  pinMode\(latchPin, OUTPUT\);

  pinMode\(dataPin, OUTPUT\);  

  pinMode\(clockPin, OUTPUT\);

  pinMode\(outputEnablePin, OUTPUT\); 

  }

/\*

* loop\(\) - this function runs over and over again

  \*/

  void loop\(\) 

  {

  setBrightness\(255\);

  leds = 0;        // Initially turns all the LEDs off, by giving the variable 'leds' the value 0

  updateShiftRegister\(\);

  delay\(500\);

  for \(int i = 0; i &lt; 8; i++\)    // Turn all the LEDs ON one by one.

  {

   bitSet\(leds, i\);        // Set the bit that controls that LED in the variable 'leds'

   updateShiftRegister\(\);

   delay\(500\);

  }

  for \(byte b = 255; b &gt; 0; b--\)    // Gradually fade all the LEDs back to off

  {

   setBrightness\(b\);

   delay\(50\);

  }

  }

/\*

* updateShiftRegister\(\) - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.

  \*/

  void updateShiftRegister\(\)

  {

  digitalWrite\(latchPin, LOW\);

  shiftOut\(dataPin, clockPin, LSBFIRST, leds\);

  digitalWrite\(latchPin, HIGH\);

  }

/\*

* setBrightness\(\) - Generates PWM signal and writes it to OE pin.

  \*/

  void setBrightness\(byte brightness\) // 0 to 255

  {

  analogWrite\(outputEnablePin, 255-brightness\);

  }

Once you upload the code to the Arduino, you should see output appear as follows: 74HC595 Shift Register PWM Brightness Control Sketch Output

