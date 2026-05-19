/******************************************************************
 * 09_NFC_RFID-Reader.ino
 * Read NFC tags and RFID cards (I2C mode)
 * turn on I2C mode by switching physical switches on the PN532 to 1 / 0 (I2C)
 * Anschluss:
 * PN532: SDA <-> ESP32-C6: GPIO 6
 * PN532: SCL <-> ESP32-C6: GPIO 7
 * PN532: Vcc <-> ESP32-C6: 3.3V
 * PN532: GND <-> ESP32-C6: GND
 * Installiere Library "Adafruit_PN532" von Adafruit
********************************************************************/


#include <Wire.h>
#include <Adafruit_PN532.h>

// I2C Pins definieren
#define SDA_PIN 6
#define SCL_PIN 7

#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET, &Wire);

// Globale Variablen für die Erkennung und Speicherung
String currentTagID = "";          // Hier wird die ID als Text ohne Leerzeichen gespeichert
String lastTagID = "";             // Merkt sich den letzten erkannten Tag

// Timer-Variablen für das 250ms Scan-Intervall
unsigned long previousScanMillis = 0;
const long scanInterval = 250;     // Alle 250ms scannen

// Timer-Variablen für die 5-Sekunden-Sperre derselben ID
unsigned long tagDetectedMillis = 0;
const long lockDuration = 5000;    // 5 Sekunden Sperrzeit

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(100);
  
  Wire.begin(SDA_PIN, SCL_PIN);
  nfc.begin();
  
  if (!nfc.getFirmwareVersion()) {
    Serial.println("Kein PN532 gefunden!");
    while (1);
  }
  
  nfc.SAMConfig();
}

void loop(void) {
  unsigned long currentMillis = millis();

  // Prüfen, ob 250ms seit dem letzten Scan vergangen sind
  if (currentMillis - previousScanMillis >= scanInterval) {
    previousScanMillis = currentMillis; // Zeitstempel aktualisieren

    uint8_t uid[7];
    uint8_t uidLength;

    // Scan mit knackigem 30ms Timeout, da wir über millis() steuern
    bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30);

    if (success) {
      // 1. ID in String ohne Leerzeichen umwandeln
      String detectedID = "";
      for (uint8_t i = 0; i < uidLength; i++) {
        if (uid[i] < 0x10) detectedID += "0";
        detectedID += String(uid[i], HEX);
      }
      detectedID.toUpperCase();

      // 2. Logik für die Wiedererkennung nach 5 Sekunden
      // Ein Tag wird gedruckt, wenn:
      // a) Es ein komplett neuer Tag ist ODER
      // b) Es derselbe Tag ist, aber die 5 Sekunden Sperrzeit abgelaufen sind
      if (detectedID != lastTagID || (currentMillis - tagDetectedMillis >= lockDuration)) {
        
        currentTagID = detectedID;
        lastTagID = detectedID;
        tagDetectedMillis = currentMillis; // 5-Sekunden-Timer neu starten

        // Einmalige Ausgabe im seriellen Port
        Serial.println(currentTagID);
      }
      
    } else {
      // Wenn kein Tag aufliegt, löschen wir die aktuelle ID-Variable.
      // lastTagID bleibt bestehen, damit die 5-Sekunden-Sperre für diesen Tag aktiv bleibt!
      currentTagID = ""; 
    }
  }

  // Hier unten kannst du nun komplett blockierungsfrei anderen Code ausführen!
  // 'currentTagID' ist während des Auflegens befüllt und wird sofort "" wenn der Tag weggezogen wird.
}