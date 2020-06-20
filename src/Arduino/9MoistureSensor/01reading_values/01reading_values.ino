const int soilMoisturePin = PA6;

void setup() {
    Serial.begin(9600);
    pinMode(soilMoisturePin, INPUT);
}

void loop() {
    int soilMoistureStatus = analogRead(soilMoisturePin);
    Serial.print("Value: ");
    Serial.println(soilMoistureStatus);
    delay(100);
}
