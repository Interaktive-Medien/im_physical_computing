
#include <Arduino.h>

// Anleitung für den ESP32-C6 auf https://youtu.be/nfZFbF84n3A

int ledPin = LED_BUILTIN;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(800);                      // wait for a second
  digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
  delay(200);                      // wait for a second
}

