/******************************************************************************
 *  17_Waage.ino
 *  Wägesensor braucht das Treibermodul HX711. 
 *  Installiere Library "HX711 Arduino Library" by Bogdan Necula
 *  HX711: DT (data)           <->   ESP32-C6: GPIO 6
 *  HX711: SCK (serial clock)  <->   ESP32-C6: GPIO 7
 *  Driver: Vcc                <->   ESP32-C6: 3.3V
 *  Driver: GND                <->   ESP32-C6: GND
 *  GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/09_Sensoren_testen/17_Waage/17_Waage.ino
 *******************************************************************************/


#include "HX711.h"

// Pinbelegung anpassen, falls nötig
#define HX711_DT  6  // Datenpin
#define HX711_SCK 7   // Clockpin

HX711 scale;

// Faktor zum Umrechnen der Rohwerte in Gramm
float calibration_factor = -7050.0;  // Muss für deinen Sensor kalibriert werden!

void setup() {
  Serial.begin(115200);
  Serial.println("HX711 Kalibrierung starten...");

  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(calibration_factor);  // Kalibrierungsfaktor setzen
  scale.tare();  // Gewicht auf der Waage als "0" setzen

  Serial.println("Wägezelle bereit. Bitte nichts auflegen.");
  delay(2000);
}

void loop() {
  if (scale.is_ready()) {
    float gewicht = scale.get_units(5);  // Mittelwert aus 5 Messungen
    Serial.print("Gewicht: ");
    Serial.print(gewicht, 1);  // 1 Nachkommastelle
    Serial.println(" g");
  } else {
    Serial.println("Wägezelle nicht bereit");
  }
  delay(1000);
}
