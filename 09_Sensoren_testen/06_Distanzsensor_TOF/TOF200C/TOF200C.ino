/**********************************************************************************************
*  distanz.ino  (Test/Demo)
*  VL53L0X Time-of-Flight Distanzsensor (TOF200C) — Distanz in mm.
*  Referenz fuer mc/distanz.h.
*  Library: VL53L0X by Pololu V1.3.1
*  Sensor: VIN<->3.3V  GND<->GND  SDA<->GPIO6  SCL<->GPIO7
*  Messbereich ca. 30-2000 mm. Status 4 = ausser Reichweite.
***********************************************************************************************/

#include <VL53L0X.h>

// Sensor-Instanz erstellen
VL53L0X sensor;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Wire.begin(6,7);

  // Sensor initialisieren
  if (!sensor.init()) {
    Serial.println(F("VL53L0X nicht gefunden!"));
    while (1);
  }
}

void loop() {
  // Entfernung messen
  uint16_t distance = sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred()) {
    Serial.println(F("Zeitüberschreitung bei Messung!"));
  } else {
    Serial.print(F("Entfernung: "));
    Serial.print(distance);
    Serial.println(F(" mm"));
  }

  delay(1000); // Kurze Pause zwischen Messungen
}