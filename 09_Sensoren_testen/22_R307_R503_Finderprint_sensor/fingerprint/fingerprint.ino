/***************************************************
  fingerprint.ino
  Fingerprint Sensor Erkennungs-Programm (Read/Test)
  Angepasst für Waveshare ESP32-C6 N8
  Zuvor müssen Fingerprints mit enroll.ino aufgenommen werden.
  
  Verkabelung:
  - Sensor TX (Schwarz) -> ESP32-C6 GPIO 6 (RX)
  - Sensor RX   -> ESP32-C6 GPIO 7 (TX)
  - Sensor V+    -> ESP32-C6 3V3
  - Sensor GND   -> ESP32-C6 GND
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// Pins für das Waveshare ESP32-C6 Board definieren
#define FINGERPRINT_RX 6  // An Sensor TX (Schwarz)
#define FINGERPRINT_TX 7  // An Sensor RX (Gelb)

// Hardware-Serial 1 für den ESP32-C6 initialisieren
HardwareSerial mySerial(1); 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(115200); // Auf 115200 angepasst für schnelles Debugging
  while (!Serial)
    ; // Warten auf die USB-Verbindung zum PC
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // Initialisiert Serial1 mit Baudrate, Signal-Konfiguration und den zugewiesenen Pins (6 und 7)
  mySerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);

  // Danach die Library auf dieser Schnittstelle starten
  finger.begin(57600);
  delay(5);
  
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

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  } else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
}

void loop() // läuft endlos durch
{
  getFingerprintID();
  delay(50); // Entlastet den Prozessor ein wenig
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image taken");
    break;
  case FINGERPRINT_NOFINGER:
    // Im normalen Loop spamt das sonst das Terminal zu, daher hier stumm geschaltet
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imaging error");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // OK, Bild erfolgreich eingelesen!

  p = finger.image2Tz();
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

  // Abgleich mit der Datenbank im Sensor
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // Match gefunden!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  return finger.fingerID;
}

// Schnellere Suchvariante (wird im Standard-Loop oben aktuell nicht aufgerufen)
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -1;

  // Match gefunden!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}