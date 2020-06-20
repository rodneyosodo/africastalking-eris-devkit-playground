const int ldrPin = PA6;

void setup() {
    Serial.begin(9600);
    pinMode(ldrPin, INPUT);
}

void loop() {
    int ldrStatus = analogRead(ldrPin);
    Serial.print("Value: ");
    Serial.println(ldrStatus);
    delay(100);
}
