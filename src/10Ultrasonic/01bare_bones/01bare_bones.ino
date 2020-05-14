// print distance to serial — hy_sr05
// Ty Seyoum

#define ECHOPIN PB11 // Pin to receive echo pulse
#define TRIGPIN PB10 // Pin to send trigger pulse

void setup() {
  Serial.begin(9600); //
  pinMode(PC13, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
}

void loop() {
  digitalWrite(PC13, LOW);
  digitalWrite(TRIGPIN, LOW); // Set the trigger pin to low for 2uS
  delay(1);
  digitalWrite(TRIGPIN, HIGH); // Send a 10uS high to trigger ranging
  delay(1);
  digitalWrite(TRIGPIN, LOW); // Send pin low again
  int distance = pulseIn(ECHOPIN, HIGH); // Read in times pulse
  distance = distance / 58; // Calculate distance from time of pulse
  if (distance <= 50) {
    digitalWrite(PC13, HIGH);
  }
  else {
    digitalWrite(PC13, LOW);
  }
  Serial.println(distance);
  delay(1000); // Wait 50mS before next ranging
}
