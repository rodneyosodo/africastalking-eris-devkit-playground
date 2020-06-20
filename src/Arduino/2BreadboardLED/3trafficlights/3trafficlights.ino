// This turns on the onboard LED for 1s and off for 0.5s repeatedly

int redLedPin = PA7;
int amberLedPin = PA6;
int greenLedPin = PB1;
int waitTimeRed = 4000;
int waitTimeAmber = 3000;
int waitTimeGeen = 3000;


void setup(){
    // initialize the digital pin as output
  pinMode(redLedPin, OUTPUT);     
  pinMode(amberLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  digitalWrite(amberLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(waitTimeRed);               // wait for a 1 second
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(amberLedPin, HIGH);    // turn the LED off by making the voltage LOW
  delay(waitTimeAmber);               // wait for a 0.5 second
  digitalWrite(redLedPin, LOW);
  digitalWrite(amberLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);    // turn the LED off by making the voltage LOW
  delay(waitTimeGeen);               // wait for a 0.5 second

}
