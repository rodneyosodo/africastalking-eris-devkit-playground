#include "pitches.h"
 
// notes in the melody:
int melody[] = {NOTE_B0, NOTE_FS1, NOTE_CS2, NOTE_GS2,NOTE_D3, NOTE_AS3, NOTE_G5, NOTE_A5, NOTE_E4, NOTE_FS5, NOTE_B5, NOTE_AS6, NOTE_DS8};
int duration = 500;  // 500 miliseconds
int buzzerPin = PA7;
void setup() {
 
}
 
void loop() {  
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // buzzerPin output the voice, every scale is 0.5 sencond
    tone(buzzerPin, melody[thisNote], duration);
     
    // Output the voice after several minutes
    delay(1000);
  }
   
  // restart after two seconds 
  delay(2000);
}
