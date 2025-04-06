/**********************************************************************************************
*  06_Distanzsensor_TOF.ino
*  TOF050C Time-of-flight sensor mit V16180x
*  Messe Entfernung zwischen 0 und 50cm und drucke sie auf den seriellen Port
*  Installiere Adafruit_VL6180X Library von Adafruit.
*  4 Anschlüsse an ESP32-C6 (Kommunikation per I2C -> dafür braucht es die vorinstallierte Wire-Library):
*  Sensor: Vin  <->  ESP32-C6: 3.3V 
*  Sensor: GND  <->  ESP32-C6: GND
*  Sensor: SDA  <->  ESP32-C6: GPIO6
*  Sensor: SCL  <->  ESP32-C6: GPIO7
***********************************************************************************************/

#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7);             // I2C mit benutzerdefinierten Pins: SDA = GPIO6, SCL = GPIO7

  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
}

void loop() {
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); 
    Serial.println(range);
  }

  delay(100);
}
