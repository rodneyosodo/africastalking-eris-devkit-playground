#include <DHT.h>

int dhtPin = PA6;
float humidity, temperature, heatIndex;

#define DHTTYPE DHT11

DHT dht(dhtPin, DHTTYPE);

void setup(){
    Serial.begin(9600);
    Serial.println("Starting DHT11");
    dht.begin();
}

void loop(){
    // Wait for a few seconds to measure
    delay(1000);
    // Reading takes about 250 milliseconds
    humidity = dht.readHumidity();
    // Read temperatire ad Celsius(default)
    temperature = dht.readTemperature();
    // Check if reads failed and exit
    if (humidity == NULL || temperature == NULL){
        Serial.println("Failed to read from DHT");
        return;
    }
    Serial.print("\nHumidity: ");
    Serial.print(humidity);
    Serial.print("%\tTemperature: ");
    Serial.print(temperature);
    Serial.print("°C");

}
