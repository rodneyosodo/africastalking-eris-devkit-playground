int ledPin = PC13;

void setup(){
    // initialize the digital pin as output
  pinMode(ledPin, OUTPUT);     
}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the ledPin on (HIGH is the voltage level)
}
