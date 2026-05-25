// FingerprintSensor_backend.h


#include <Adafruit_Fingerprint.h>

// Hardware-Serial 1 für den ESP32-C6 initialisieren
HardwareSerial mySerial(1); 

#define FINGERPRINT_RX 6  // An Sensor TX 
#define FINGERPRINT_TX 7  // An Sensor RX 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id = 1;             // ID 1 dient als temporärer Slot im Sensor, da die echte ID von der DB vergeben wird
String fingerprintHex = ""; // Hier wird die gesamte Kette gespeichert


void initFingerprintSensor(){
  Serial.println("huhu");

  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  mySerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);      // Initialisiert Serial1 mit Baudrate, Signal-Konfiguration und den zugewiesenen Pins (6 und 7)
  finger.begin(57600);   // Danach die Library auf dieser Schnittstelle starten

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);
}

uint8_t readTypedNumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (!Serial.available())
      ;
    num = Serial.parseInt();
  }
  return num; // z.B. 3
}


uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
    delay(50); // Sensor etwas Atempause geben
  }

  // Erster Scan erfolgreich!

  p = finger.image2Tz(1);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
    delay(50);
  }

  // Zweiter Scan erfolgreich!

  p = finger.image2Tz(2);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // Beide Scans abgleichen und Modell erstellen
  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // Modell im Flash des Sensors speichern
  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    
    // --- KORREKTUR: In globale Variable schreiben ---
    Serial.println("Lade das soeben erstellte Template aus dem Sensor-Zwischenspeicher...");
    
    p = finger.getModel(); 
    if (p == FINGERPRINT_OK) {
      Serial.println("Sensor sendet Datenpaket. Verarbeite Daten...");
      
      int byteCount = 0;
      bool datenVorhanden = true;

      while (datenVorhanden) {
        uint32_t timeout = millis();
        // Warte bis zu 50ms auf das nächste Byte im seriellen Buffer
        while (mySerial.available() == 0) {
          if (millis() - timeout > 50) {
            datenVorhanden = false; // Keine Daten mehr nachgekommen, wir sind fertig
            break;
          }
          delay(1);
        }

        if (datenVorhanden) {
          uint8_t c = mySerial.read();
          
          // Fuege führende Null hinzu, wenn der Wert kleiner als 16 ist
          if (c < 0x10) {
            fingerprintHex += "0";
          }
          fingerprintHex += String(c, HEX);
          byteCount++;
        }
      }
      
      Serial.print("Uebertragene Bytes gesamt: ");
      Serial.println(byteCount);
      
    } else {
      Serial.print("Fehler beim Holen des Templates: ");
      Serial.println(p);
    }
    // ------------------------------------------------------------------------------

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}