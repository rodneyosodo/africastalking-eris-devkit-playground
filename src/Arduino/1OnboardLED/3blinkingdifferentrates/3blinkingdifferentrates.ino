// This turns on the onboard LED for 1s and off for 0.5s repeatedly

int ledPin = PC13;
int waitTimeOn = 1000;
int waitTimeOff = 500;

void setup(){
    // initialize the digital pin as output
  pinMode(ledPin, OUTPUT);     
}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(waitTimeOn);               // wait for a 1 second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(waitTimeOff);               // wait for a 0.5 second
}
