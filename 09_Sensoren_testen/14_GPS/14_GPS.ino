/***************************************************
 *  14_GPS.ino
 *  Kommunikation zw. GPS-Modul und ESP32-C6 via UART
 *  Installiere Library TinyGPSPlus by Mikal Hart
 *  Verbinde:
 *  GPS-Modul: TX  <-> ESP32-C6: GPIO6 (RX)
 *  GPS-Modul: RX  <-> ESP32-C6: GPIO7 (TX)
 *  GPS-Modul: Vcc <-> ESP32-C6: 3V3
 *  GPS-Modul: GND <-> ESP32-C6: GND
 *  sollte das GPS Modul noch weitere Pins haben, einfach ignorieren
 *  GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/09_Sensoren_testen/14_GPS/14_GPS.ino
 ***************************************************/

#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Instanz der TinyGPS++ Bibliothek
TinyGPSPlus gps;

// HardwareSerial für GPS-Daten
HardwareSerial SerialGPS(1); // UART1 für das GPS-Modul
float latitude = 0;
float longitude = 0;
float altitude = 0;
String timeString = ""; 
int satellites = 0;

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
  if (gps.location.isUpdated() && gps.location.isValid()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }

  if (gps.altitude.isUpdated()) {
    altitude = gps.altitude.meters();
  }
  if (gps.satellites.isUpdated()) {
    satellites = gps.satellites.value();
  }

  if (gps.date.isUpdated() && gps.time.isUpdated()) {
    // KORREKTUR: MariaDB DATETIME Format (YYYY-MM-DD HH:MM:SS)
    char timeBuf[32];
    snprintf(timeBuf, sizeof(timeBuf), "%04d-%02d-%02d %02d:%02d:%02d", 
             gps.date.year(), gps.date.month(), gps.date.day(), 
             gps.time.hour(), gps.time.minute(), gps.time.second());
    timeString = String(timeBuf); 
  }

  Serial.printf("Breitengrad: %.6f\n", latitude );
  Serial.printf("Längengrad: %.6f\n", longitude );
  Serial.printf("Höhe: %.2f m\n", altitude );
  Serial.printf("Satelliten: %d\n", satellites );
  Serial.printf("Zeit/Datum: %s\n", timeString.c_str() ); 
  Serial.println("----------------------------");
}