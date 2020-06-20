#include <Servo.h> 
int pos; 
Servo myservo;
 
void setup() 
{ 
  myservo.attach(PA7);
  myservo.writeMicroseconds(1500);  // set servo to mid-point
} 
 
void loop() {
    for(pos = 0; pos <= 3000; pos++ ) // goes from 0 degrees to 180 degrees
    { // in steps of 1 degree
        myservo.writeMicroseconds(pos); // tell servo to go to position in variable 'pos'
        delay(15); // waits 15ms for the servo to reach the position
    }
    for(pos = 3000; pos>= 0; pos-- ) // goes from 180 degrees to 0 degrees
    {
        myservo.writeMicroseconds(pos); // tell servo to go to position in variable 'pos'
        delay(15); // waits 15ms for the servo to reach the position
    }
}
