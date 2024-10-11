/****************************************************************************
* use RFID Reader RFID-RC522 with ESP32-C6
* Install library RFID_MFRC522v2
* verbinde ESP32-C6 und RFID-RC522 wie folgt
*  -  Clock am RFID-RC522  <->  GPIO 16 am ESP32-C6
*  -  MISO am RFID-RC522   <->  GPIO 2 am ESP32-C6
*  -  MOSI am RFID-RC522   <->  GPIO 7 am ESP32-C6
*  -  SDA am RFID-RC522    <->  GPIO 5 am ESP32-C6
*  -  Reset am RFID-RC522  <->  Rst am ESP32-C6
*  -  3.3V am RFID-RC522   <->  3.3V am ESP32-C6
*  -  GND am RFID-RC522    <->  GND am ESP32-C6
*****************************************************************************/


#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <SPI.h>

// Definiere die Pins für SPI
#define SDA_PIN 5        // SDA/SS Pin (GPIO 5)
#define SCK_PIN 6       // SPI Clock (GPIO 6 fix beim ESP32-C6, 11 beim UNO)
#define MISO_PIN 2      // SPI MISO (GPIO 2 fix beim ESP32-C6, 12 beim UNO)
#define MOSI_PIN 7      // SPI MOSI (GPIO 7 fix beim ESP32-C6, 13 beim UNO)

SPIClass spi = SPI;
MFRC522DriverPinSimple ss_pin(SDA_PIN); 
MFRC522DriverSPI driver(ss_pin, spi);
MFRC522 reader(driver); 

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialisiere die SPI-Verbindung mit den richtigen Pins
  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SDA_PIN); // SCK, MISO, MOSI, SS

  // Initialisiere den RFID-Reader
  reader.PCD_Init();
  Serial.print(F("Reader: "));
  MFRC522Debug::PCD_DumpVersionToSerial(reader, Serial);
}

void loop() {
  // Überprüfe, ob eine neue Karte vorhanden ist.
  if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {
    // Die UID in eine String-Variable konvertieren
    String uidString = "";
    for (byte i = 0; i < reader.uid.size; i++) {
      if (reader.uid.uidByte[i] < 0x10) uidString += "0"; // Füge führende Null hinzu, falls nötig
      uidString += String(reader.uid.uidByte[i], HEX); // Byte in Hex-String umwandeln
    }
    
    // UID ausgeben
    Serial.println(uidString);
    
    // Verwende die UID weiter nach Bedarf
    // Beispiel: if (uidString == "a1b2c3d4") { ... }
    
    // Beende Kommunikation mit der Karte
    reader.PICC_HaltA();
    reader.PCD_StopCrypto1();
  }
}