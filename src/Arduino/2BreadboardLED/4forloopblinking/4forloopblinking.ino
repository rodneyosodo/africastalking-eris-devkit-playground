// This turns on the onboard LED for 1s and off for 0.5s repeatedly

int redLedPin = PA7;
int yellowLedPin = PA6;
int redWaitTimeOn = 300;
int redWaitTimeOff = 400;
int yellowWaitTimeOn = 250;
int yellowWaitTimeOff = 300;
int redNumBlink = 5;
int yellowNumBlink = 3;
int i,j ;

void setup(){
    // initialize the digital pin as output
  pinMode(redLedPin, OUTPUT);     
  pinMode(yellowLedPin, OUTPUT);
}

void loop() {
    for(i = 0 ; i <= redNumBlink ; i++ ){
        digitalWrite(redLedPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(redWaitTimeOn);               // wait for a 1 second
        digitalWrite(redLedPin, LOW);    // turn the LED off by making the voltage LOW
        delay(redWaitTimeOff); 
    }
    for(j = 0 ; j <= yellowNumBlink ; j++ ){
        digitalWrite(yellowLedPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(yellowWaitTimeOn);               // wait for a 1 second
        digitalWrite(yellowLedPin, LOW);    // turn the LED off by making the voltage LOW
        delay(yellowWaitTimeOff); 
    }
}
