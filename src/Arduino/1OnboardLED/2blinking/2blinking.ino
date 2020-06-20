// This turns on the onboard LED for 0.1s repeatedly

int ledPin = PC13;
int waitTime = 1000;

void setup(){
    // initialize the digital pin as output
  pinMode(ledPin, OUTPUT);     
}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(waitTime);               // wait for a 1 second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(waitTime);               // wait for a 1 second
}
