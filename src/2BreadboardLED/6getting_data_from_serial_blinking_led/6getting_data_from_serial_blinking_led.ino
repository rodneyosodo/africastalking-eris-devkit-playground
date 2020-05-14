// This turns on the onboard LED for 1s and off for 0.5s repeatedly

int redLedPin = PA7;
int yellowLedPin = PA6;
int redWaitTimeOn = 400;
int redWaitTimeOff = 300;
int yellowWaitTimeOn = 200;
int yellowWaitTimeOff = 200;
int redNumBlink;
int yellowNumBlink;
int i,j;
String redMessage = "\n\nThe Red LED is blinking";
String yellowMessage = "\nThe Yellow LED is blinking\n";
String blinkMessage = "\tYou are on blink : ";


void setup(){
    // initialize the digital pin as output
  pinMode(redLedPin, OUTPUT);     
  pinMode(yellowLedPin, OUTPUT);
  Serial.begin(9600);

  Serial.println("How many times do you want the RED led to blink: ");
  while(Serial.available() == 0){  }
  redNumBlink = Serial.parseInt();

  Serial.println("How many times do you want the YELLOW led to blink: ");
  while(Serial.available() == 0){  }
  yellowNumBlink = Serial.parseInt();
}

void loop() {
    Serial.println(redMessage);
    for(i = 0 ; i <= redNumBlink ; i++){
        Serial.print(blinkMessage);
        Serial.println(i);
        digitalWrite(redLedPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(redWaitTimeOn);               // wait for a 1 second
        digitalWrite(redLedPin, LOW);    // turn the LED off by making the voltage LOW
        delay(redWaitTimeOff); 
    }
    Serial.print(yellowMessage + blinkMessage);
    for(j = 0 ; j <= yellowNumBlink ; j++){
        Serial.print(j);
        digitalWrite(yellowLedPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(yellowWaitTimeOn);               // wait for a 1 second
        digitalWrite(yellowLedPin, LOW);    // turn the LED off by making the voltage LOW
        delay(yellowWaitTimeOff); 
    }
}
