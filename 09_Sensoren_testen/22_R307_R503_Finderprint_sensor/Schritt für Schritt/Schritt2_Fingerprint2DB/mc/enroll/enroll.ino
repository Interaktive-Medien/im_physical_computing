/***************************************************
  Fingerprint Sensor Einlese-Programm (Enrollment)
  Finger Print Sensor R307 / R503
  Adapted for ESP32-C6-N8 from Library example files
  Install Adafruit-Fingerprint-Sensor-Library by Adafruit
  
  Verkabelung:
  - Sensor TX  <-> ESP32-C6 GPIO 6 (RX)
  - Sensor RX  <-> ESP32-C6 GPIO 7 (TX)
  - Sensor V+  <-> ESP32-C6 3V3
  - Sensor GND <-> ESP32-C6 GND
 ****************************************************/

#include <HTTPClient.h>
#include <Arduino_JSON.h> 

const char* serverURL = "https://fingerprint.dorfkneipe.ch/api/load.php";  // Server-Adresse
int led = LED_BUILTIN;

#include "FingerprintSensor_backend.h"
#include "WLAN.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(led, OUTPUT);
  rgbLedWrite(led, 0, 255, 0);                      // GRB: rot
  Serial.println("Starte Verbindung...");
  connectWiFi();                                    // in WLAN.h

  initFingerprintSensor();
}

void loop() {
  // Loop abbrechen, falls kein WLAN da ist
  if (!is_wlan_connected()) return; 

  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Lege deinen Finger auf den Sensor, um das Enrollment zu starten...");

  // Bleibt hier in der Schleife, bis ein Finger erfolgreich zweimal gescannt wurde
  while (!getFingerprintEnroll()) {
    delay(100);
  }

  // Wenn wir hier ankommen, steht der Hex-String in 'fingerprintHex' bereit
  Serial.println("Hier ist der fertige Hex-String fuer dein JSON:");
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

  // String leeren für das nächste Enrollment im folgenden Loop-Durchlauf
  fingerprintHex = "";
  
  Serial.println("\n--------------------------------------------------\n");
  delay(3000); // Kurze Pause vor dem nächsten Durchlauf
}