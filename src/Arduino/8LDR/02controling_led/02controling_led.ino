const int ledPin = PC13;

const int ldrPin = PA6;

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(ldrPin, INPUT);
}

void loop() {
    int ldrStatus = analogRead(ldrPin);
    if (ldrStatus >= 2000) {
        digitalWrite(ledPin, HIGH);
        Serial.print("Its DARK, LED is on : ");
        Serial.println(ldrStatus);
    } else {
        digitalWrite(ledPin, LOW);
        Serial.print("Its BRIGHT, LED is off : ");
        Serial.println(ldrStatus);
    }
    delay(100);
}
