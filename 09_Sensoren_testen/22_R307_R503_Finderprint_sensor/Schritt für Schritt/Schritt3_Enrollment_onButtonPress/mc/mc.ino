/***************************************************
  Fingerprint Sensor Einlese-Programm (Enrollment)
  Finger Print Sensor R307 / R503
  Adapted for ESP32-C6-N8 from Library example files
  Install Adafruit-Fingerprint-Sensor-Library by Airuino
  
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

const char* serverURL = "https://fingerprint.dorfkneipe.ch/api/load.php";  // Server-Adresse
int led = LED_BUILTIN;
const int buttonPin = 10;

#include "FingerprintSensor_backend.h"
#include "WLAN.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(led, OUTPUT);
  rgbLedWrite(led, 0, 255, 0);        // grb rot
  pinMode(buttonPin, INPUT_PULLDOWN); // Taster als Input mit internem Pulldown
  Serial.println("Starte Verbindung...");
  connectWiFi();                                    // in WLAN.h
  initFingerprintSensor();                          // in FingerprintSensor_backend.h
}

void loop() {
  if (!is_wlan_connected()) return; 
  Serial.println("Warte auf Tastendruck (Pin 8), um Enrollment zu starten...");
  
  
  ////////////////////////////////////////////////////////////// Daten erheben -> Nach Tasterdruck fingerprint Sensor auslesen 

  while (digitalRead(buttonPin) == LOW) {.        // Warte, bis der Taster gedrückt wird (HIGH)
    delay(50);
    if (!is_wlan_connected()) return; // Sicherheits-Check im Standby
  }

  rgbLedWrite(led, 0, 0, 255); // LED blau (Bereit für Finger)
  Serial.println("Taster gedrueckt! 10-Sekunden-Zeitfenster gestartet.");

  unsigned long startTime = millis();
  bool enrollSuccess = false;

  while (millis() - startTime < 10000) {       // 10 Sekunden (10000 ms) Zeitfenster für den Fingerabdruck
    if (getFingerprintEnroll()) {
      enrollSuccess = true;
      break; // Schleife vorzeitig verlassen, da Enrollment erfolgreich war
    }
    delay(50);
  }

  if (enrollSuccess) {
    Serial.println("ID als Hex-String:");
    Serial.println(fingerprintHex);

    ////////////////////////////////////////////////////////////// JSON zusammenbauen

    JSONVar dataObject;
    dataObject["fingerprintHex"] = fingerprintHex;
    String jsonString = JSON.stringify(dataObject);

    ////////////////////////////////////////////////////////////// JSON per HTTP POST senden

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");
      
      Serial.println("Sende Daten an den Server...");
      int httpResponseCode = http.POST(jsonString);

      // Prüfen der Antwort
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        Serial.println("Response: " + response);
      } else {
        Serial.printf("Error on sending POST: %d\n", httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }

    fingerprintHex = "";      // String leeren für das nächste Enrollment
  } else {
    Serial.println("Timeout erreicht! Kein Finger innerhalb von 10s erkannt.");
  }

  resetStatusLed();           // in WLAN.h: Nach dem Vorgang (oder Timeout) LED wieder auf Standard-WLAN-Status zurücksetzen

  Serial.println("\n--------------------------------------------------\n");
  delay(2000); // Kurze Pause, um dauerhaftes Triggern beim Loslassen zu verhindern
}