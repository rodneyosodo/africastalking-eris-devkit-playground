const int ledPin = PC13;

const int soilMoisturePin = PA6;
int analogValue;
int soilMoistureValue;

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(soilMoisturePin, INPUT);
}

void loop() {
    int analogValue = analogRead(soilMoisturePin);
    soilMoistureValue = map(analogValue, 0, 4095, 0, 100);	//Map value : 400 will be 100 and 1023 will be 0
    if (soilMoistureValue >= 50) {
        digitalWrite(ledPin, HIGH);
        Serial.print("High soil moisture: ");
        Serial.println(soilMoistureValue);
    } else {
        digitalWrite(ledPin, LOW);
        Serial.print("Low soil moisture: ");
        Serial.println(soilMoistureValue);
    }
    delay(100);
}
