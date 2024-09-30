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