/**********************************************************************************************
*  Distanzsensor_TOF050C.ino
*  TOF050C Time-of-flight sensor mit V16180x
*  optimale Entfernungs-Range: 0 - 22cm
*  Installiere Adafruit_VL6180X Library von Adafruit.
*  Anschluss an ESP32-C6 (Kommunikation per I2C -> dafür braucht es die vorinstallierte Wire-Library):
*  Sensor: Vin  <->  ESP32-C6: 3.3V 
*  Sensor: GND  <->  ESP32-C6: GND
*  Sensor: SDA  <->  ESP32-C6: GPIO6
*  Sensor: SCL  <->  ESP32-C6: GPIO7
*  Sensor: INT  <->  ESP32-C6: nicht verbunden
*  Sensor: SHUT <->  ESP32-C6: nicht verbunden
***********************************************************************************************/

#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

void setup() {
  Serial.begin(115200);

  // I2C mit benutzerdefinierten Pins initialisieren
  Wire.begin(6, 7); // SDA = GPIO6, SCL = GPIO7

  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
}

void loop() {
  // float lux = vl.readLux(VL6180X_ALS_GAIN_5);

  // Serial.print("Lux: "); Serial.println(lux);
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); Serial.println(range);
  }

  delay(100);
}
