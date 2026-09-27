/******************************************************************************
 *  PROGRAMM 1: Kalibrierung & Speicherung
 *******************************************************************************/

#include "Adafruit_HX711.h"
#include <Preferences.h>

const int HX_DT = 6;
const int HX_SCK = 7;

Adafruit_HX711 hx711(HX_DT, HX_SCK);
Preferences preferences; // Instanz für den Flash-Speicher

const float refWeight = 81.0; // Dein Referenzgewicht in Gramm
int32_t tareValue = 0;
float waage_calfactor = 1.0f;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- Starte Kalibrierung ---");

  hx711.begin();

  // Sensor stabilisieren lassen
  for (int i = 0; i < 3; i++) hx711.readChannelBlocking(CHAN_A_GAIN_128);

  // 1. Tare ermitteln (Waage muss leer sein)
  Serial.println("Ermittle Tare (NICHTS auflegen)...");
  int64_t sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  tareValue = sum / 15;
  Serial.println("Tare gesetzt!");

  // 2. Auf Gewicht warten
  Serial.print("Lege jetzt das Referenzgewicht (");
  Serial.print(refWeight);
  Serial.println(" g) auf.");
  Serial.println("Du hast 10 Sekunden Zeit...");
  delay(10000);

  // 3. Referenz messen
  Serial.println("Messe Referenzgewicht...");
  sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  int32_t rawRef = sum / 15;

  // 4. Faktor berechnen
  waage_calfactor = (float)(rawRef - tareValue) / refWeight;
  Serial.print("Berechneter Faktor: ");
  Serial.println(waage_calfactor);

  // 5. Faktor dauerhaft im Flash (NVS) speichern
  preferences.begin("waage", false); // "waage" ist der Namensraum, false = Schreibzugriff
  preferences.putFloat("calfactor", waage_calfactor);
  preferences.end(); // Speicher wieder schließen

  Serial.println("Kalibrierung abgeschlossen und auf dem ESP32-C6 gespeichert!");
  Serial.println("Du kannst jetzt das zweite Programm aufspielen.");
}

void loop() {
  // Nichts zu tun im Loop.
  delay(1000);
}