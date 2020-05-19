#include <DHT.h>

#define DHTPIN PA6

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup(){
    Serial.begin(9600); //  sets up serial comm's to 9600 baud
    dht.begin();        //  initialize the DHT22
    //  display and serial print initilization
    Serial.println("DHTxx test!");
    delay(1500);
}
 
void loop(){
    // Reading temperature and humidity takes about 250 milliseconds
    float H = dht.readHumidity();        //  read humitity as %f
    float C = dht.readTemperature();     //  read temperature as Celsius (default)
    float F = dht.readTemperature(true); //  read temperature as Fahrenheit (if true)
 
    // DHT22 data error check section
    if (H == NULL || C == NULL){
        Serial.println("Failed to read from DHT sensor!");
        return;   //  returns to start of loop
    }

    //  calculations and conversions section
    float HiF = dht.computeHeatIndex(F, H); //  read and compute heat index in Fahrenheit (the default)
    float DewPoint = (C - (100 - H) / 5);   //  dewpoint calculation using Celsius value
    float DP = (DewPoint * 9 / 5) + 32;     //  converts dewPoint calculation to fahrenheit


     //  serial monitor and debug section
    Serial.print("Sample OK: ");                                //  prints sample ok to serial monitor
    Serial.print(C);        Serial.print(" °C, ");         //  prints celsius value to serial monitor and returns cursor
    Serial.print(F);        Serial.print(" °F, ");         //  prints fahrenheit value to serial monitor and returns cursor
    Serial.print(H);        Serial.print(" %, ");          //  prints humidity value to serial monitor and returns cursor
    Serial.print(DP);       Serial.print(" DP, ");         //  prints dewpoint value to serial monitor and returns cursor to next line
    Serial.print(HiF);      Serial.print(" Heat index, "); //  prints humidity value to serial monitor and returns cursor
    Serial.print(DewPoint); Serial.println(" DewP, ");     //  prints dewpoint value to serial monitor and returns cursor to next line

     // The DHT11 sampling rate is .5HZ (.5 Sec), this delay sets program sample rate to match so no errors happen
    delay(2000);
}
