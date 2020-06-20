// This turns on the onboard LED for 1s and off for 0.5s repeatedly

int ledPin = PA7;
int potentiometerPin = PA6;
int potentiometerValue;
int ledWriteValue;

void setup(){
    // initialize the digital pin as output
  pinMode(ledPin, OUTPUT);     
  pinMode(potentiometerPin, INPUT);
  Serial.begin(9600);
}

void loop() {
    potentiometerValue = analogRead(potentiometerPin);
    ledWriteValue = (255.0 / 4095.0) * potentiometerValue;
    analogWrite(ledPin, ledWriteValue);
    Serial.print("You are writing value: ");
    Serial.println(potentiometerValue);
}
