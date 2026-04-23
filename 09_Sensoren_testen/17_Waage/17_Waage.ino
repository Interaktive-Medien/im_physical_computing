/******************************************************************************
 *  17_Waage.ino
 *  Wägesensor braucht das Treibermodul HX711. 
 *  Installiere Library "HX711 Arduino Library" by Bogdan Necula
 *  HX711: DT (data)           <->   ESP32-C6: GPIO 6
 *  HX711: SCK (serial clock)  <->   ESP32-C6: GPIO 7
 *  Driver: Vcc                <->   ESP32-C6: 3.3V
 *  Driver: GND                <->   ESP32-C6: GND
 *  Vorgehensweise:
 *  Das gemessene Gewicht erscheint in einem Wertebereich, die für uns nicht gut lesbar ist.
 *  Es muss in eine für uns lesbare Range multipliziert werden. 
 *  Dazu braucht es den Kalibrationsfaktor (calFactor). Um diesen zu ermitteln, brauchen wir zur Berechnung 2 Messwerte:
 *  0 und ein bekanntes Referenzgewicht, z.B. eine Schoggi mit 100g Gewicht
 *  1. Referenzpunkt: Bei Start nichts auflegen. Tare
 *  2. Referenzpunkt: Nach Aufforderung (im serial monitor) eine 100g Tafel Schoggi auflegen. 
 *  Nun können andere Gewichte gewogen werden.
 *  Wenn der calFactor bekannt ist, kann der calFactor) hardgecoded in den Code geschrieben werden. Der Block entfällt:  if (!calibrated) {..}
 *  see https://prilchen.de/eine-waage-mit-arduino-erstellen/
 *******************************************************************************/



#include <Wire.h>
#include "HX711.h"

// HX711 Pins
const int HX_DT = 6;   // DOUT
const int HX_SCK = 7;  // SCK
HX711 scale;

// Kalibrierfaktor (wird im Schritt 2 berechnet)
float calFactor = 1.0f;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // HX711
  scale.begin(HX_DT, HX_SCK);
  scale.set_gain(128);   // Kanal A, Gain 128
  delay(200);

  // Schritt 1: Nullpunkt setzen (Tare)
  scale.tare(15);        // 15 Samples mitteln

  // Hinweis
  Serial.println("Tare gesetzt");
  Serial.println("Lege bekanntes gewicht auf");
  delay(5000);
}

void loop() {
  // Schritt 2: Kalibrierung mit Referenzgewicht
  // Beispiel: 100 g Referenzgewicht
  static bool calibrated = false;
  const float refWeight = 100.0; // Beispiel 100 Gramm Schokolade

  if (!calibrated) {
    long raw = scale.get_value(15);   // Rohwert ohne Skalierung
    calFactor = raw / refWeight;      // Faktor berechnen
    scale.set_scale(calFactor);       // setzen
    calibrated = true;

    Serial.println("Kalibrierung OK");
    char buf[32];
    Serial.println("calFactor");
    Serial.println(calFactor);
    delay(2000);
  }

  // Danach: normale Gewichtsanzeige
  float grams = scale.get_units(10); // gemittelt
  char buf[24];
  Serial.println(grams);

  delay(500);
}