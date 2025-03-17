/******************************************************************
 * Read NFC tags and RFID cards (I2C mode)
 * turn on I2C mode by switching physical switches to 1 / 0
 * PN532: SDA -> ESP32-C6: GPIO 6
 * PN532: SCL -> ESP32-C6: GPIO 7
 * Put libraries PN532, PN532_I2C, NDEF into Documents>Arduino>libraries 
 * (download on https://github.com/elechouse/PN532)
********************************************************************/

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

volatile bool connected = false;
String prevDetected;
long prevDetectedTimestamp = 0;

void setup(void){
  Serial.begin(115200);
  Serial.println("*** Testing Module PN532 NFC RFID ***");
  Wire.begin(6, 7);  // SDA = Pin 6, SCL = Pin 7 für ESP32-C6
}

void loop(void){
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };     // Buffer to store the UID
  uint8_t uidLength;                           // UID size (4 or 7 bytes depending on card type)

  while (!connected) {
    connected = connect();
  }
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success){
    String uidString = "";
    for (uint8_t i = 0; i < uidLength; i++){
      uidString += String(uid[i], HEX);
    }
    if (!prevDetected.equals(uidString) || (millis() - prevDetectedTimestamp) > 2000) {
      Serial.println(uidString);
      prevDetected = uidString;
      prevDetectedTimestamp = millis();
    }
  }
}

bool connect() {
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.println("PN532 card not found!");
    return false;
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware version: "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  Serial.println("Waiting for card (ISO14443A Mifare)...");
  Serial.println("");
  return true;
}