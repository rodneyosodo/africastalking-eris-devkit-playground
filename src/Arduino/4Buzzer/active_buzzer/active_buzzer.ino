int buzzerPin = PA7;

void setup(){
    pinMode(buzzerPin, OUTPUT);
}
void loop(){
    unsigned char i;
    while (1)
    {
        /* code */
        for (i = 0; i < 80; i++)
        {
            digitalWrite(buzzerPin, HIGH);
            delay(1);
            digitalWrite(buzzerPin, LOW);
            // output another frequency
            for (i = 0; i < 100; i++)
            {
                digitalWrite(buzzerPin, HIGH);
                delay(2);
                digitalWrite(buzzerPin, LOW); 
                delay(2);
                for (i = 0; i < 100; i++)
                {
                  digitalWrite(buzzerPin, HIGH);
                  delay(3);
                  digitalWrite(buzzerPin, LOW); 
                  delay(3);
                  for (i = 0; i < 100; i++)
                {
                  digitalWrite(buzzerPin, HIGH);
                  delay(4);
                  digitalWrite(buzzerPin, LOW); 
                  delay(4);
                 }
                 }
            }
            
        }
        
    }
    
}
