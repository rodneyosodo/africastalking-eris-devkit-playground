// This uses Serial Monitor to display Range Finder distance readings

// Hook up HC-SR04 with Trig to Arduino Pin 10, Echo to Arduino pin 13
// Maximum Distance is 400 cm

// This uses Serial Monitor to display Range Finder distance readings

// Include NewPing Library
#include <NewPing.h>

// Hook up HC-SR04 with Trig to Arduino Pin 9, Echo to Arduino pin 10
#define TRIGGER_PIN PB10
#define ECHO_PIN PB11

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400	

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;

void setup() 
{
	Serial.begin(9600);
}

void loop() 
{
	// Send ping, get distance in cm
	distance = sonar.ping_cm();
    // duration = sonar.ping();
    // distance = (duration / 2) * 0.0343;
	
	// Send results to Serial Monitor
	Serial.print("Distance = ");
	
	if (distance >= 400 || distance <= 2) 
	{
		Serial.println("Out of range");
	}
	else 
	{
		Serial.print(distance);
		Serial.println(" cm");
	}
	delay(500);
}