/***************************************************************
 * 03_Drive_servo.ino
 * Drehe Servo vor und zurück im Loop
 * Installiere Library "ESP32Servo" by Kevin Harrington et al.
 * Verbinde ...
 * Servo: Vin   <->  external + line
 * Servo: GND   <->  external - line
 * Servo: Data  <->  ESP32-C6: GPIO2
 ***************************************************************/

#include <ESP32Servo.h>
Servo myServo;
int servoPin = 2;
void setup() {
  myServo.attach(servoPin);
}

void loop() {
  for (int pos = 0; pos <= 180; pos++) {
    myServo.write(pos);  // Position setzen
    delay(15);           // Kleine Verzögerung für sanfte Bewegung
  }

  for (int pos = 180; pos >= 0; pos--) {
    myServo.write(pos);  // Position setzen
    delay(15);           // Kleine Verzögerung für sanfte Bewegung
  }
}