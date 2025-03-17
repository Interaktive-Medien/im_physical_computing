/***************************************************
 *  GPS.ino
 *  Kommunikation zw. GPS-Modul und ESP32-C6 via UART
 *  Installiere Library TinyGPSPlus by Mikal Hart
 *  Verbinde:
 *  GPS-Modul: TX  <-> ESP32-C6: GPIO6 (RX)
 *  GPS-Modul: RX  <-> ESP32-C6: GPIO7 (TX)
 *  GPS-Modul: Vcc <-> ESP32-C6: 3V3
 *  GPS-Modul: GND <-> ESP32-C6: GND
 *  sollte das GPS Modul noch weitere Pins haben, einfach ignorieren
 ***************************************************/

#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Instanz der TinyGPS++ Bibliothek
TinyGPSPlus gps;

// HardwareSerial für GPS-Daten
HardwareSerial SerialGPS(1); // UART1 für das GPS-Modul

void setup() {
  Serial.begin(115200);        // Serielle Kommunikation mit PC
  SerialGPS.begin(9600, SERIAL_8N1, 6, 7); // GPS: Baudrate 9600, RX=GPIO6, TX=GPIO7 
  Serial.println("GPS-Modul wird initialisiert...");
}

void loop() {
  while (SerialGPS.available() > 0) {
    char c = SerialGPS.read(); // Zeichen vom GPS-Modul lesen
    if (gps.encode(c)) {       // NMEA-Daten dekodieren
      displayGPSData();        // GPS-Daten anzeigen
    }
  }
}

// Funktion, um GPS-Daten auf der Konsole auszugeben
void displayGPSData() {
  if (gps.location.isUpdated()) {
    Serial.printf("Breitengrad: %.6f", gps.location.lat() );
    Serial.printf("Längengrad: %.6f", gps.location.lng() );
  }
  if (gps.date.isUpdated()) {
    Serial.printf("Datum: %02d/%02d/%04d", gps.date.day(), gps.date.month(), gps.date.year() );
  }
  if (gps.time.isUpdated()) {
    Serial.printf("Uhrzeit (UTC): %02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second() );
  }
  if (gps.altitude.isUpdated()) {
    Serial.printf("Höhe: %.2f m", gps.altitude.meters() );
  }
  if (gps.satellites.isUpdated()) {
    Serial.printf("Satelliten: %d", gps.satellites.value() );
  }
}
