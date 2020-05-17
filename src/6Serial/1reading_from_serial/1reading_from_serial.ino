String userName;
int userAge;
float userMass;

void setup(){
    Serial.begin(500000);
}

void loop(){
    Serial.println("\nPlease enter your name: ");
    while (Serial.available() == 0){
    }
    userName = Serial.readString();

    Serial.println("Please enter your age: ");
    while (Serial.available() == 0){
    }
    userAge = Serial.parseInt();

    Serial.println("Please enter your mass in kg: ");
    while (Serial.available() == 0){
    }
    userMass = Serial.parseFloat();

    Serial.print("\nHello, ");
    Serial.print(userName);
    Serial.print(" you are ");
    Serial.print(userAge);
    Serial.print(" years old and weight\n1. Kilograms: ");
    Serial.print(userMass);
    Serial.print("\n2. Grams: ");
    Serial.print(userMass*1000);
    Serial.print("\n3. Tonnes: ");
    Serial.print(userMass/1000.0);
    
}
