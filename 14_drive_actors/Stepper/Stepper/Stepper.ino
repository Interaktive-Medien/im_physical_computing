/**************************************
 *  Stepper.ino
 *  Based on Stepper Motor Control - one revolution by Tom Igoe
 *  Der Stepper 28BYJ-48 ist über den Stepper-Driver ULN2003  mit dem ESP32-C6 verbunden
 *  Verbinde IN1-4 am Stepper-Driver mit GPIO18-21
 *  see https://randomnerdtutorials.com/esp32-stepper-motor-28byj-48-uln2003/ 
 ****************************************/
#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 18
#define IN2 19
#define IN3 20
#define IN4 21

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  // set the speed at 5 rpm
  myStepper.setSpeed(17);
  // initialize the serial port
  Serial.begin(115200);
}

void loop() {
  // step one revolution in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution*2);
  delay(1000);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution*2);
  delay(1000);
}