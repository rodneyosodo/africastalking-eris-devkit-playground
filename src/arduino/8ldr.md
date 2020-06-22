# README

We all want our home appliances to be controlled automatically based on some conditions and that's called Home automation. Today we are going to control the light based of darkness outside, the light turns ON automatically when it is dark outside and turns off when it gets bright. For this, we need a light sensor to detect the light condition and some circuitry to control the Light sensor. It’s like Dark and light Detector circuit but this time we are using Arduino to get more control over light.

In this circuit, we are making a Light Sensor using LDR with Arduino to control a bulb/CFL as per light condition of the room or outside area.

Material Required Arduino UNO LDR \(Light Dependent Resistor\) Resistor \(100k-1;330ohm-1\) LED - 1 Relay module - 5v Bulb/CFL Connecting wires Breadboard Circuit Diagram

LDR LDR is Light Dependent Resistor. LDRs are made from semiconductor materials to enable them to have their light-sensitive properties. There are many types but one material is popular and it is cadmium sulfide \(CdS\). These LDRs or PHOTO RESISTORS works on the principle of “Photo Conductivity”. Now what this principle says is, whenever light falls on the surface of the LDR \(in this case\) the conductance of the element increases or in other words, the resistance of the LDR falls when the light falls on the surface of the LDR. This property of the decrease in resistance for the LDR is achieved because it is a property of semiconductor material used on the surface.

We previously made many Circuits using LDR, which use LDR to automate the lights according to requirement.

Working of LDR controlled LED using Arduino As per the circuit diagram, we have made a voltage divider circuit using LDR and 100k resistor. The voltage divider output is feed to the analog pin of the Arduino. The analog Pin senses the voltage and gives some analog value to Arduino. The analog value changes according to the resistance of LDR. So, as the light falls on the LDR the resistance of it get decreased and hence the voltage value increase.

Intensity of light ↓ - Resistance↑ - Voltage at analog pin↓ - Light turns ON

As per the Arduino code, if the analog value falls below 700 we consider it as dark and the light turns ON. If the value comes above 700 we consider it as bright and the light turns OFF.

Code Explanation: Complete Arduino Code and Demonstration Video is given at the end of this project.

Here, we are defining the Pins for Relay, LED and LDR.

## define relay 10

int LED = 9; int LDR = A0; Setting up the LED and Relay as Output pin, and LDR as input pin.

pinMode\(LED, OUTPUT\); pinMode\(relay, OUTPUT\); pinMode\(LDR, INPUT\); Reading the voltage analog value through the A0 pin of the Arduino. This analog Voltage will be increased or decreased according to the resistance of LDR.

int LDRValue = analogRead\(LDR\); Giving the condition for dark and bright. If the value is less than 700 then it is dark and the LED or Light turns ON. If the value is greater than 700 then it is bright and the LED or light turns OFF.

if \(LDRValue &lt;=700\) { digitalWrite\(LED, HIGH\); digitalWrite\(relay, HIGH\); Serial.println\("It's Dark Outside; Lights status: ON"\); } else { digitalWrite\(LED, LOW\); digitalWrite\(relay, LOW\); Serial.println\("It's Bright Outside; Lights status: OFF"\); } Controlling Relay using LDR with Arduino ​

Instead of controlling an LED according to the brightness and darkness, we can control our home lights or any electrical equipment. All we have to do is connect a relay module and set the parameter to turn ON and OFF the any AC appliance according to the intensity of the light. If the value falls below 700, which means it Dark, then the relay operates and the lights turns ON. If the value is greater than 700, which means its day or bright, then the relay will not operate and the lights remain OFF. Learn more about relay here and how to connect an AC appliance to relay.

Also, check:

Automatic Street Light Controller Circuit Using Relay and LDR Automatic Staircase Light Raspberry Pi Emergency Light Code

## define relay 10

int LED = 9; int LDR = A0;

void setup\(\) { Serial.begin\(9600\); pinMode\(LED, OUTPUT\); pinMode\(relay, OUTPUT\); pinMode\(LDR, INPUT\); }

void loop\(\) { int LDRValue = analogRead\(LDR\); Serial.print\("sensor = "\); Serial.print\(LDRValue\);

if \(LDRValue &lt;=700\) { digitalWrite\(LED, HIGH\); digitalWrite\(relay, HIGH\); Serial.println\("It's Dark Outside; Lights status: ON"\); } else { digitalWrite\(LED, LOW\); digitalWrite\(relay, LOW\); Serial.println\("It's Bright Outside; Lights status: OFF"\); } }

i Everyone, This is my 1st Arduino's tutorial video. In this video I am going to show you how to use a LDR or Light Dependent resistor to turn on and off another circuit or a LED.

Wouldn’t it be really cool if whenever a room gets dark, a light bulb automatically turns ON and eliminates the darkness? In this very simple project, I am focusing on eliminating darkness. You can even use this as an emergency lighting system.

Instructions Connect the 3.3v output of the Arduino to the positive rail of the breadboard Connect the ground to the negative rail of the breadboard Place the LDR on the breadboard Attach the 10K resistor to one of the legs of the LDR Connect the A0 pin of the Arduino to the same column where the LDR and resistor is connected \(Since the LDR gives out an analog voltage, it is connected to the analog input pin on the Arduino. The Arduino, with its built-in ADC \(Analog to Digital Converter\), then converts the analog voltage from 0-5V into a digital value in the range of 0-1023\). - Now connect the other end of the 10K resistor to the negative rail And the the second \(free\) leg of the LDR to the positive rail Pretty much this is what we need for the light sensing. Basic circuits like this can be done without an Arduino aswell. However, if you want to log the values and use it to create charts, run other logics etc. I will recomend an Arduino or ESP8266 or may be a ESP32 for this.

Now, as we want our circuit to do something in the real world other than just displaying the values on the computer screen we will be attaching a LED to the circuit. The LED will turn on when its dark and will go off when its bright. To achieve this we will:

Place the LED on the breadboard Connect the 220ohm resistor to the long leg \(+ve\) of the LED Then we will connect the other leg of the resistor to pin number 13 \(digital pin\) of the Arduino and the shorter leg of the LED to the negative rail of the breadboard

