/**********************************************************************************************
* BMP280_Druck_und_Hoehe.ino
* BMP280 Luftdruck- und Höhenmessung
* Gibt den barometrischen Luftdruck und die berechnete Höhe im Seriellen Monitor aus.
*
* Benötigt die Library: "Adafruit BMP280 Library"
*  4 Anschlüsse 
*  Sensor: VCC  <->  ESP32-C6: 3.3V
*  Sensor: GND  <->  ESP32-C6: GND
*  Sensor: SDA  <->  ESP32-C6: GPIO6
*  Sensor: SCL  <->  ESP32-C6: GPIO7
***********************************************************************************************/

#include <Wire.h>
#include <Adafruit_BMP280.h>

// I2C-Pins für den ESP32-C6 definieren
#define I2C_SDA 6
#define I2C_SCL 7

// Referenz-Luftdruck auf Meereshöhe (Standardwert bei Normalnull: 1013.25 hPa)
// Für eine exakte Höhenmessung kann dieser Wert an das aktuelle Wetter angepasst werden.
const float SEALEVEL_HPA = 1013.25;

Adafruit_BMP280 bmp; // I2C-Objekt erstellen

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(100); // Warten bis der serielle Monitor offen ist

  Serial.println(F("--- BMP280 Sensor-Test ---"));

  // I2C-Bus mit den Pins des ESP32-C6 initialisieren
  Wire.begin(I2C_SDA, I2C_SCL);

  // Sensor mit I2C-Adresse initialisieren (0x76 ist der Standard für die meisten Module)
  if (!bmp.begin(0x76)) {
    Serial.println(F("Fehler: Kein BMP280-Sensor gefunden! Verkabelung prüfen."));
    while (1) delay(10);
  }

  // Sensor-Einstellungen für normale Messungen konfigurieren
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,     // Temperatur-Oversampling
                  Adafruit_BMP280::SAMPLING_X16,    // Luftdruck-Oversampling
                  Adafruit_BMP280::FILTER_X16,      // Interner Filter gegen Rauschen
                  Adafruit_BMP280::STANDBY_MS_500); // Messintervall
}

void loop() {
  // 1. Luftdruck auslesen und von Pascal (Pa) in Hektopascal (hPa) umrechnen
  float luftdruck = bmp.readPressure() / 100.0F;

  // 2. Höhe basierend auf dem Referenz-Luftdruck berechnen lassen
  float hoehe = bmp.readAltitude(SEALEVEL_HPA);

  // Ausgabe im Seriellen Monitor
  Serial.print(F("Luftdruck: "));
  Serial.print(luftdruck);
  Serial.println(F(" hPa"));

  Serial.print(F("Berechnete Höhe: "));
  Serial.print(hoehe);
  Serial.println(F(" m"));
  
  Serial.println(F("-----------------------"));

  // 2 Sekunden warten bis zur nächsten Messung
  delay(2000);
}
