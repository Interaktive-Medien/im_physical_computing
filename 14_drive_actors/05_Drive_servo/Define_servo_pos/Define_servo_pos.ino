/***************************************************************
 * Drive_servo_pos.ino
 * Schicke eine Zahl an die serielle Schnittstelle und der Servo fährt dorthin
 * Installiere Library "ESP32Servo" by Kevin Harrington et al.
 * Verbinde ...
 * Servo: Vin   <->  external + line
 * Servo: GND   <->  external - line
 * Servo: Data  <->  ESP32-C6: GPIO2
 ***************************************************************/


#include <ESP32Servo.h>

Servo myServo;
int servoPin = 5;

void setup(){
  Serial.begin(115200);
  myServo.attach(servoPin);
}

void loop(){
  if(Serial.available()){

    String input = Serial.readStringUntil('\n'); // Lese die Eingabe bis zum Zeilenumbruch
    input.trim();
    int angle = input.toInt();
    myServo.write(angle);
    Serial.println(angle);
  }
}