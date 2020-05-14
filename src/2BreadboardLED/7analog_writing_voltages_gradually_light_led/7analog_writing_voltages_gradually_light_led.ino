// This turns on the onboard LED for 1s and off for 0.5s repeatedly

int whiteLedPin = PA6;
int whiteWaitTimeOn = 5;
int whiteWaitTimeOff = 10;
int whiteNumBlink = 2;
int blueLedPin = PA7;
int blueWaitTimeOn = 5;
int blueWaitTimeOff = 10;
int blueNumBlink = 1;
int i,j ;

void setup(){
    // initialize the digital pin as output
  pinMode(whiteLedPin, OUTPUT);     
    pinMode(blueLedPin, OUTPUT);     

}

void loop() {
  for (i = 0; i <= whiteNumBlink; i++){
    for(j = 0 ; j <= 255 ; j++ ){
        analogWrite(whiteLedPin, j);   // turn the LED on (HIGH is the voltage level)
        delay(whiteWaitTimeOn);               // wait for a 1 second
    }
    analogWrite(whiteLedPin, 0);    // turn the LED off by making the voltage LOW
    delay(whiteWaitTimeOff); 
   }
  for (i = 0; i <= blueNumBlink; i++){
    for(j = 0 ; j <= 255 ; j++ ){
        analogWrite(blueLedPin, j);   // turn the LED on (HIGH is the voltage level)
        delay(blueWaitTimeOn);               // wait for a 1 second
    }
    analogWrite(blueLedPin, 0);    // turn the LED off by making the voltage LOW
    delay(blueWaitTimeOff); 
   }

}