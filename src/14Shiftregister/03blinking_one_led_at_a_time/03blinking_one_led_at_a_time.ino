int latchPin = 5;	// Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6;	// Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4;	// Data pin of 74HC595 is connected to Digital pin 4

byte leds = 0;		// Variable to hold the pattern of which LEDs are currently turned on or off
int currentLED = 0;

void setup()
{
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    leds = 0;
}

void loop()
{
    leds = 0;

    if (currentLED == 7)
    {
        currentLED = 0;
    }
    else
    {
        currentLED++;
    }

    bitSet(leds, currentLED);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, leds);
    digitalWrite(latchPin, HIGH);

    delay(250);
}
