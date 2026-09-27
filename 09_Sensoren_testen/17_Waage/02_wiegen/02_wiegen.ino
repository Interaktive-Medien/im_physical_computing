/******************************************************************************
 *  PROGRAMM 2: Waage im Normalbetrieb
 *******************************************************************************/

#include "Adafruit_HX711.h"
#include <Preferences.h>

const int HX_DT = 6;
const int HX_SCK = 7;

Adafruit_HX711 hx711(HX_DT, HX_SCK);
Preferences preferences;

int32_t tareValue = 0;
float waage_calfactor = 1.0f;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- Waage startet ---");

  // 1. Gespeicherten Faktor abrufen
  preferences.begin("waage", true); // true = reiner Lesezugriff (schont den Flash)
  // Lese "calfactor". Falls er nicht existiert, nimm als Fallback 1.0f
  waage_calfactor = preferences.getFloat("calfactor", 1.0f);
  preferences.end();

  Serial.print("Geladener Kalibrierungsfaktor: ");
  Serial.println(waage_calfactor);

  if (waage_calfactor == 1.0f) {
    Serial.println("WARNUNG: Faktor ist 1.0. Wurde das Kalibrierungsprogramm ausgeführt?");
  }

  hx711.begin();

  // 2. Waage nullen (Automatisches Tare beim Einschalten)
  Serial.println("Waage nullt sich... (bitte nichts auflegen)");
  for (int i = 0; i < 3; i++) hx711.readChannelBlocking(CHAN_A_GAIN_128); // Stabilisieren
  
  int64_t sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  tareValue = sum / 15;
  
  Serial.println("Waage ist bereit!");
}

void loop() {
  // Aktuellen Rohwert lesen
  int32_t rawValue = hx711.readChannelBlocking(CHAN_A_GAIN_128);
  
  // Umrechnung: (Aktueller Wert - Nullpunkt) / Faktor
  float grams = (rawValue - tareValue) / waage_calfactor;
  
  Serial.print("Gewicht: ");
  Serial.print(grams, 1); // , 1 bedeutet eine Nachkommastelle
  Serial.println(" g");
  
  delay(500); // Messintervall
}