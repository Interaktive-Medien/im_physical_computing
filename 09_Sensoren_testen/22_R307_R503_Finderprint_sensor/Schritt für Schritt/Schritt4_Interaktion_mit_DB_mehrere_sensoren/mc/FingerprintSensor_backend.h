// FingerprintSensor_backend.h

#include <Adafruit_Fingerprint.h>

HardwareSerial mySerial(1); 

#define FINGERPRINT_RX 6  
#define FINGERPRINT_TX 7  
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id = 1;             
String fingerprintHex = ""; 

void initFingerprintSensor(){
  Serial.println("Adafruit Fingerprint sensor initialization");
  Serial.println("Taster drücken, um neuen fingerprint aufzunehmen");
  mySerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);      
  finger.begin(57600);   

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}

// Hardware-Vergleich von Slot 1 (aktueller Finger) und Slot 2 (die jeweiligen Finger aus der DB über eine Schleife)
uint16_t matchTemplateCustom() {
  while(mySerial.available() > 0) {
    mySerial.read();
  }

  // Protokoll fuer R503 Match-Befehl (0x03)
  uint8_t cmdPacket[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x03, 0x00, 0x07};
  mySerial.write(cmdPacket, sizeof(cmdPacket));
  
  uint32_t timeout = millis();
  while (mySerial.available() < 12) {
    if (millis() - timeout > 500) {
      return 0; 
    }
    delay(1);
  }
  
  uint8_t reply[12];
  for (int i = 0; i < 12; i++) {
    reply[i] = mySerial.read();
  }
  
  if (reply[9] == 0x00) {
    uint16_t score = (reply[10] << 8) | reply[11];
    return score;
  }
  
  return 0; 
}

// Hilfsfunktion: Streamt die Sensor-Daten des aktuellen Scans in den ESP-String
bool downloadTemplate() {
  uint8_t p = finger.getModel(); 
  if (p == FINGERPRINT_OK) {
    int byteCount = 0;
    bool datenVorhanden = true;

    while (datenVorhanden) {
      uint32_t timeout = millis();
      while (mySerial.available() == 0) {
        if (millis() - timeout > 50) {
          datenVorhanden = false;
          break;
        }
        delay(1);
      }

      if (datenVorhanden) {
        uint8_t c = mySerial.read();
        if (c < 0x10) fingerprintHex += "0";
        fingerprintHex += String(c, HEX);
        byteCount++;
      }
    }
    return true;
  }
  return false;
}

// KORREKTUR: Sendet den Ankündigungsbefehl, bevor die DB-Bytes gestreamt werden
bool uploadTemplateToSlot2(String hexString) {
  if (hexString.length() < 20) return false;

  // 1. Dem Sensor sagen, dass wir ein Template in Slot 2 (CharBuffer 2) hochladen moechten
  // Befehl 0x09 (DownLoadModel) an Buffer 0x02
  // Paket: Start(EF01) + Addr(FFFFFFFF) + Typ(01) + Len(0004) + Befehl(09) + Buffer(02) + Checksum(0010)
  uint8_t downloadCmd[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x09, 0x02, 0x00, 0x10};
  
  // Befehl senden
  mySerial.write(downloadCmd, sizeof(downloadCmd));
  
  // Kurz auf die Bestaetigung des Sensors warten (Acknowledge)
  uint32_t timeout = millis();
  while (mySerial.available() < 12) {
    if (millis() - timeout > 100) return false;
    delay(1);
  }
  
  // Antwort lesen und pruefen, ob der Sensor bereit ist (Byte 9 == 0x00)
  uint8_t reply[12];
  for (int i = 0; i < 12; i++) {
    reply[i] = mySerial.read();
  }
  if (reply[9] != 0x00) return false; // Sensor meldet Fehler oder ist nicht bereit

  // 2. Jetzt, wo der Sensor im Empfangsmodus ist, wandeln wir den DB-Hex-String in Bytes um
  int len = hexString.length() / 2;
  uint8_t bytes[len];

  for (int i = 0; i < len; i++) {
    String byteString = hexString.substring(i * 2, (i * 2) + 2);
    bytes[i] = (uint8_t) strtol(byteString.c_str(), NULL, 16);
  }

  // 3. Das komplette Paket aus der Datenbank direkt hinterherstreamen
  mySerial.write(bytes, len);
  delay(40); // Dem Sensor etwas Zeit geben, das große Datenpaket im RAM zu verarbeiten
  
  return true;
}

// Liest den Finger ein (Egal ob für Registrierung oder Login) und legt Merkmale in Slot 1 ab
bool getFingerprintScan() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return false;

  p = finger.image2Tz(1); // Merkmale in Slot 1 sichern
  if (p != FINGERPRINT_OK) return false;
  return downloadTemplate();
}

// Holt alle Vorlagen von readUsers.php und vergleicht sie hardwareseitig -> wird aufgerufen, bevor neuer User eingetragen wird bzw. bei Login
int checkFingerAgainstDatabase() {
  if (WiFi.status() != WL_CONNECTED) return -1;

  HTTPClient http;
  http.begin(readUsersURL); 
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    JSONVar myObject = JSON.parse(response);

    if (JSON.typeof(myObject) != "undefined" && myObject.hasOwnProperty("users")) {
      JSONVar usersList = myObject["users"];

      for (int i = 0; i < usersList.length(); i++) {
        int dbId = (int)usersList[i]["id"];
        String dbHex = (String)usersList[i]["fingerprintHex"];
        Serial.print("testing entry ");
        Serial.println(i);

        if (uploadTemplateToSlot2(dbHex)) {
          uint16_t confidence = matchTemplateCustom();
          Serial.printf(" -> Match Score mit Datenbank-ID %d: %d\n", dbId, confidence);
          
          if (confidence >= 100) { 
            http.end();
            return dbId; 
          }
        } else {
          Serial.println(" -> Fehler beim Upload des Templates in den Sensor.");
        }
      }
    }
  }
  http.end();
  return -1; 
}