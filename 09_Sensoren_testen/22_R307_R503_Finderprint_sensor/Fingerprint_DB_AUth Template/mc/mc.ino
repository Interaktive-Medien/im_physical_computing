/***************************************************
  Fingerprint Sensor Einlese-Programm (Enrollment & Login)
  Finger Print Sensor R307 / R503
  Adapted for ESP32-C6-N8 from Library example files
  
  Verkabelung:
  - Fingerprint Sensor TX  <-> ESP32-C6 GPIO 6 (RX)
  - Fingerprint Sensor RX  <-> ESP32-C6 GPIO 7 (TX)
  - Fingerprint Sensor V+  <-> ESP32-C6 3V3
  - Fingerprint Sensor GND <-> ESP32-C6 GND
  - Taster Input           <-> ESP32-C6 GPIO 10

    Passe an:
  - serverURL
 ****************************************************/

#include <HTTPClient.h>
#include <Arduino_JSON.h> 

const char* loadURL   = "https://fingerprint.dorfkneipe.ch/api/load.php";  // Erstellt neuen User
const char* unloadURL = "https://fingerprint.dorfkneipe.ch/api/unload.php"; // Holt alle Vorlagen (neueste zuerst)
const char* loginURL  = "https://fingerprint.dorfkneipe.ch/api/login.php";  // Trägt Login in History ein

int led = LED_BUILTIN;
const int buttonPin = 10;

#include "WLAN.h"
#include "FingerprintSensor_backend.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(led, OUTPUT);
  rgbLedWrite(led, 0, 255, 0);        // GRB: Rot beim Start
  pinMode(buttonPin, INPUT_PULLDOWN); 
  
  Serial.println("Starte Verbindung...");
  connectWiFi();                      
  initFingerprintSensor();            
}

void loop() {
  if (!is_wlan_connected()) return; 

  // ==========================================
  // PHASE 1: ENROLLMENT MODUS (Taster gedrückt)
  // ==========================================
  if (digitalRead(buttonPin) == HIGH) {
    rgbLedWrite(led, 0, 0, 255); // LED blau (Bereit für Finger)
    Serial.println("\n[ENROLLMENT] Taster gedrueckt! 10s-Zeitfenster gestartet.");

    unsigned long startTime = millis();
    bool enrollSuccess = false;

    while (millis() - startTime < 10000) { 
      if (getFingerprintScan()) { // Liest Finger in Slot 1 und speichert in 'fingerprintHex'
        enrollSuccess = true;
        break; 
      }
      delay(50);
    }

    if (enrollSuccess) {
      rgbLedWrite(led, 100, 255, 0); // GRB: Orange (Prüfphase)
      Serial.println("[ENROLLMENT] Finger gescannt. Pruefe auf Duplikate in der DB...");

      // 1. Alle vorhandenen Finger aus der DB laden, um sie mit dem aktuellen Finger zu vergleichen
      int existingUserId = checkFingerAgainstDatabase();

      if (existingUserId != -1) {
        // Finger existiert bereits! Abbruch.
        Serial.printf("[-] ENROLLMENT ABGEBROCHEN: Dieser Finger ist bereits als User-ID %d registriert!\n", existingUserId);
      } else {
        // Finger ist neu -> ab in die Datenbank damit!
        Serial.println("[+] Finger ist neu! Sende an load.php...");

        JSONVar dataObject;
        dataObject["fingerprintHex"] = fingerprintHex;
        String jsonString = JSON.stringify(dataObject);

        if (WiFi.status() == WL_CONNECTED) {
          HTTPClient http;
          http.begin(loadURL);
          http.addHeader("Content-Type", "application/json");
          int httpResponseCode = http.POST(jsonString);

          if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Server Response: " + response);
          } else {
            Serial.printf("HTTP-Fehler bei load.php: %d\n", httpResponseCode);
          }
          http.end();
        }
      }
      fingerprintHex = ""; 
    } else {
      Serial.println("[-] Timeout erreicht! Kein Finger innerhalb von 10s erkannt.");
    }
    
    resetStatusLed();
    delay(1000); 
  }
  
  // ==========================================
  // PHASE 2: AUTOMATISCHER LESEMODUS (Standard)
  // ==========================================
  else {
    if (getFingerprintScan()) { // Finger aufgelegt und extrahiert?
      rgbLedWrite(led, 100, 255, 0); // GRB: Orange (Verarbeitungsphase)
      Serial.println("\n[LOGIN] Finger erkannt! Starte lokalen Abgleich mit DB-Templates...");

      // Nutzt dieselbe Funktion wie die Doppelungsprüfung
      int matchedUserId = checkFingerAgainstDatabase();

      if (matchedUserId != -1) {
        Serial.printf("[»»»] MATCH GEFUNDEN! Ergreife Login fuer User-ID: %d\n", matchedUserId);
        
        // Sende die ermittelte User-ID an den Server, damit dieser die History befüllt
        JSONVar loginObject;
        loginObject["user_id"] = matchedUserId;
        String jsonString = JSON.stringify(loginObject);

        if (WiFi.status() == WL_CONNECTED) {
          HTTPClient http;
          http.begin(loginURL);
          http.addHeader("Content-Type", "application/json");
          int httpResponseCode = http.POST(jsonString);

          if (httpResponseCode > 0) {
            Serial.println("Server Response (History): " + http.getString());
          }
          http.end();
        }
      } else {
        Serial.println("[»»»] VERIFIZIERUNG FEHLGESCHLAGEN: Unbekannter Finger!");
      }
      
      fingerprintHex = ""; 
      resetStatusLed();
      delay(2000); // Sperrzeit vor dem nächsten Scan
    }
  }
  
  delay(50); 
}