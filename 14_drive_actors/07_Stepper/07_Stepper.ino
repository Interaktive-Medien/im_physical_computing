/******************************************************************************
 *  07_Stepper.ino
 *  Drehe eine Umdrehung vor und zurück
 *  Stepper Library ist standardmässig vorinstalliert in der Arduino IDE
 *  Der Stepper 28BYJ-48 ist über den Stepper-Driver ULN2003  mit dem ESP32-C6 verbunden
 *  Verbinde IN1-4 am Stepper-Driver mit GPIO18-21 am ESP32-C6
 *  Stepper Driver: IN1   <->  ESP32-C6: GPIO21
 *  Stepper Driver: IN2   <->  ESP32-C6: GPIO20
 *  Stepper Driver: IN3   <->  ESP32-C6: GPIO19
 *  Stepper Driver: IN4   <->  ESP32-C6: GPIO18
 *  see https://randomnerdtutorials.com/esp32-stepper-motor-28byj-48-uln2003/ 
 *******************************************************************************/

#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 21
#define IN2 20
#define IN3 19
#define IN4 18

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