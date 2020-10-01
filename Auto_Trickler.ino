// Include the servo library:
#include <Servo.h>

// Create a new servo object:
Servo TricklerServo;

// Define the servo pin:
#define servoPin 9
// Define the sensor pin
#define SensorPin A5
// Define the fast potentiometer pin
#define FastPin A3
// Define the button pin
#define ButtonPin A2
// Define the LED pin
#define LEDPin 7


void setup() {
  // Attach the Servo variable to a pin:
  TricklerServo.attach(servoPin);
  pinMode(LEDPin, OUTPUT);
}

void loop() {
TricklerServo.write(90);

//Define photo sensor pin
int SensorValue = analogRead(SensorPin);
float SensorVoltage = SensorValue * (5.0 / 1023.0);

//Define fast servo speed cutout voltage setting potentiometer
int FastValue = analogRead(FastPin);
float FastVoltage = FastValue * (5.0 / 1023.0);

digitalWrite(LEDPin, LOW);

//wait for the button press before getting the sensor calibration voltage
//zero out the scale and wait till it is settled out and then hit the button
//blink the red light while it waits to calibrate
while (digitalRead(ButtonPin) == LOW) {
  delay(25); 
  digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW
  delay(25); 
  }

int count = 0;
float CalSum = 0;
float CalValue = 0;
float CalVoltage;

//calibrate the sensor with 50 samples
while (count<50) {
  CalValue = CalValue + analogRead(SensorPin);
  count = count + 1;
  delay(25); 
  digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW
  delay(25); 
  }

//calculate the calibration voltage
CalVoltage = CalValue / 50 * (5.0 / 1023.0);

//loop the dispenser forever
while(1){
//wait for the button press before dispensing powder
digitalWrite(LEDPin, LOW); 
 
while (digitalRead(ButtonPin) == LOW) {
  delay(10);
}
  digitalWrite(LEDPin, HIGH); 
//read the sensor voltage
SensorValue = analogRead(FastPin);
SensorVoltage = SensorValue * (5.0 / 1023.0);

//check if the sensor voltage is greater than the cal voltage, dispense powder till within 0.5V of the cal at fast speed
while (SensorVoltage > CalVoltage + 0.5) {
    SensorValue = analogRead(SensorPin);
    SensorVoltage = SensorValue * (5.0 / 1023.0);
    FastValue = analogRead(FastPin);
    FastVoltage = FastValue * (5.0 / 1023.0);
    TricklerServo.write(90+FastVoltage*2);
    delay(10);
}
//check if the sensor voltage is greater than the cal voltage, dispense powder till the cal voltage at slow speed
  while (SensorVoltage > CalVoltage) {
    SensorValue = analogRead(SensorPin);
    SensorVoltage = SensorValue * (5.0 / 1023.0);
    TricklerServo.write(93);
    delay(10);
}

//stop servo
TricklerServo.write(90);
}
}
