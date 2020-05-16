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
	Serial.print("Soil humidity: ");
	Serial.print(soilMoistureValue);
	Serial.println("%");
    delay(1000);
}
