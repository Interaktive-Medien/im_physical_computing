#include <Wire.h>
#include "Adafruit_VL6180X.h"

#define SDA_PIN 6
#define SCL_PIN 7

#define XSHUT1 10  // Sensor 1 XSHUT an GPIO10
#define XSHUT2 11  // Sensor 2 XSHUT an GPIO11

Adafruit_VL6180X sensor1 = Adafruit_VL6180X();
Adafruit_VL6180X sensor2 = Adafruit_VL6180X();

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // XSHUT-Pins als Ausgang setzen
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);

  // Beide Sensoren ausschalten
  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  delay(10);

  // Sensor 1 aktivieren
  digitalWrite(XSHUT1, HIGH);
  delay(10);
  if (!sensor1.begin(0x30)) {  // neue Adresse für Sensor 1
    Serial.println("Sensor 1 nicht gefunden");
    while (1);
  }

  // Sensor 2 aktivieren
  digitalWrite(XSHUT2, HIGH);
  delay(10);
  if (!sensor2.begin(0x31)) {  // neue Adresse für Sensor 2
    Serial.println("Sensor 2 nicht gefunden");
    while (1);
  }

  Serial.println("Beide Sensoren initialisiert.");
}

void loop() {
  uint8_t range1 = sensor1.readRange();
  uint8_t status1 = sensor1.readRangeStatus();

  uint8_t range2 = sensor2.readRange();
  uint8_t status2 = sensor2.readRangeStatus();

  if (status1 == VL6180X_ERROR_NONE) {
    Serial.print("Sensor 1: ");
    Serial.print(range1);
    Serial.print(" mm\t");
  } else {
    Serial.print("Sensor 1 Fehler\t");
  }

  if (status2 == VL6180X_ERROR_NONE) {
    Serial.print("Sensor 2: ");
    Serial.print(range2);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 2 Fehler");
  }

  delay(200);
}
