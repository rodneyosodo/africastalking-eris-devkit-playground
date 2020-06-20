int latchPin = 5;		// Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6;		// Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4;		// Data pin of 74HC595 is connected to Digital pin 4
int outputEnablePin = 3;	// OE pin of 74HC595 is connected to PWM pin 3

byte leds = 0;		// Variable to hold the pattern of which LEDs are currently turned on or off

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup() 
{
  // Set all the pins of 74HC595 as OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT); 
}

/*
 * loop() - this function runs over and over again
 */
void loop() 
{
  setBrightness(255);
  leds = 0;		// Initially turns all the LEDs off, by giving the variable 'leds' the value 0
  updateShiftRegister();
  delay(500);
  for (int i = 0; i < 8; i++)	// Turn all the LEDs ON one by one.
  {
    bitSet(leds, i);		// Set the bit that controls that LED in the variable 'leds'
    updateShiftRegister();
    delay(500);
  }
  for (byte b = 255; b > 0; b--)	// Gradually fade all the LEDs back to off
  {
    setBrightness(b);
    delay(50);
  }
}

/*
 * updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
 */
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

/*
 * setBrightness() - Generates PWM signal and writes it to OE pin.
 */
void setBrightness(byte brightness) // 0 to 255
{
  analogWrite(outputEnablePin, 255-brightness);
}