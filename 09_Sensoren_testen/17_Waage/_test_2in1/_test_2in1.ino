/******************************************************************************
 *  gewicht.ino  (Test/Demo)
 *  Waegesensor mit HX711 Treibermodul.
 *  Library: "Adafruit HX711"
 * zunächst nichts auflegen, dann nach Aufforderung das Referenzgewicht auflegen, dann irgendwas
 *******************************************************************************/

#include "Adafruit_HX711.h"

const int HX_DT = 6;   // DOUT
const int HX_SCK = 7;  // SCK

// Initialisierung der Adafruit Bibliothek
Adafruit_HX711 hx711(HX_DT, HX_SCK);

int32_t tareValue = 0;
float calFactor = 1.0f;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("los gehts");

  hx711.begin();

  // Ein paar Lesevorgänge "wegwerfen", damit der Sensor nach dem Start stabil wird
  for (int i = 0; i < 3; i++) {
    hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }

  // Tare: Mittelwert aus 15 Messungen ohne Gewicht ermitteln
  Serial.println("Ermittle Tare (nichts auflegen)...");
  int64_t sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  tareValue = sum / 15;
  
  Serial.println("Tare gesetzt");
  Serial.println("Lege bekanntes Gewicht auf");
  
  // 5 Sekunden Zeit, um das Gewicht aufzulegen
  delay(5000);
}

void loop() {
  static bool calibrated = false;
  const float refWeight = 81.0; // Referenzgewicht in Gramm

  if (!calibrated) {
    // Referenzgewicht messen (wieder Mittelwert aus 15 Messungen für mehr Stabilität)
    int64_t sum = 0;
    for (int i = 0; i < 15; i++) {
      sum += hx711.readChannelBlocking(CHAN_A_GAIN_128);
    }
    int32_t rawRef = sum / 15;
    
    // waage_calfactor berechnen: (Rohwert mit Gewicht - Rohwert ohne Gewicht) / Referenzgewicht
    waage_calfactor = (float)(rawRef - tareValue) / refWeight;
    calibrated = true;
    
    Serial.println("Kalibrierung OK");
    Serial.print("waage_calfactor: ");
    Serial.println(waage_calfactor);
    delay(2000);
  }

  // Gewicht fortlaufend berechnen und ausgeben
  int32_t rawValue = hx711.readChannelBlocking(CHAN_A_GAIN_128);
  
  // Die eigentliche Umrechnung in Gramm
  float grams = (rawValue - tareValue) / waage_calfactor;
  
  Serial.print(grams);
  Serial.println(" g");
  delay(500);
}