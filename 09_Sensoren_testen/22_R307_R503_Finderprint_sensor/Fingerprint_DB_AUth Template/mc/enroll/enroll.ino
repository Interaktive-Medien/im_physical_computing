/***************************************************
  Fingerprint Sensor Einlese-Programm (Enrollment)
  Finger Print Sensor R307 / R503
  Adapted for ESP32-C6-N8 from Library example files
  Install Adafruit-Fingerprint-Sensor-Library by Adafruit
  
  Verkabelung:
  - Sensor TX  <-> ESP32-C6 GPIO 6 (RX)
  - Sensor RX  <-> ESP32-C6 GPIO 7 (TX)
  - Sensor V+  <-> ESP32-C6 3V3
  - Sensor GND <-> ESP32-C6 GND
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// Hardware-Serial 1 für den ESP32-C6 initialisieren
HardwareSerial mySerial(1); 

#define FINGERPRINT_RX 6  // An Sensor TX 
#define FINGERPRINT_TX 7  // An Sensor RX 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
String fingerprintHex = ""; // Hier wird die gesamte Kette gespeichert

#include "FingerprintSensor_backend.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  initFingerprintSensor();
}



void loop() {
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readTypedNumber();    // Programm bleibt hier erstmal stehen, bis eine Nummer per Serial Port eingegeben wurde
  if (id == 0) { // ID #0 ist nicht erlaubt
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);

  while (!getFingerprintEnroll())
    ;

  // Die Variable steht ab hier bereit. Wir geben sie zur Kontrolle einmal im Monitor aus:
  Serial.println("Hier ist der fertige Hex-String fuer dein JSON:");
  Serial.println(fingerprintHex);
}

