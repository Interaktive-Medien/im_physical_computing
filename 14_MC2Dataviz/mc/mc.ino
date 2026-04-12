/******************************************************************************************
 * Kap. 13: Sende Sensordaten an Server
 * mc.ino
 * Installiere Library "Arduino_JSON" by Arduino
 * Sensordaten sammeln und per HTTP POST Request an Server schicken (-> an load.php).
 * load.php schreibt die Werte dann in die Datenbank
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 * Gib SSID und Passwort deines WLANs an.
 * Ersetze den Block "Sensor auslesen" durch tatsächliche Sensorwerte.
 ******************************************************************************************/



#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;                                  // alle 15s wird ein neuer Wert verschickt

const char* ssid     = "tinkergarden";                             // WLAN SSID
const char* pass     = "strenggeheim";                             // WLAN Passwort
const char* serverURL = "https://<your_website.ch>/api/load.php";  // Server-Adresse: hier kann http oder https stehen, aber nicht ohne, zB. https://im4.physco.dorfkneipe.ch/api/load.php

bool isWlanConnected = 0;
int led = LED_BUILTIN;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(led, OUTPUT);
  rgbLedWrite(led, 0, 255, 0);                        // GRB: rot
  Serial.println("Starte Verbindung...");
  connectWiFi();
}

void loop() {
  if (!is_wlan_connected())return; 
  if ((millis() - lastTime) > timerDelay) {           // alle 15 Sekunden...
    lastTime = millis();

    ////////////////////////////////////////////////////////////// sensor auslesen

    float wert = (float)random(0, 1000) / 10;         // ersetzen durch sensor !! Zunächst zufällige Zahl 0 - 100
    Serial.println(wert);

    ////////////////////////////////////////////////////////////// JSON zusammenbauen

    JSONVar dataObject;
    dataObject["wert"] = wert;
    String jsonString = JSON.stringify(dataObject);
    // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77}";  // stattdessen könnte man den JSON string auch so zusammenbauen

  
     ////////////////////////////////////////////////////////////// JSON string per HTTP POST request an den Server schicken (server2db.php)

    if (WiFi.status() == WL_CONNECTED) {                // Überprüfen, ob Wi-Fi verbunden ist
      // HTTP Verbindung starten und POST-Anfrage senden
      HTTPClient http;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");
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
  }
}


void connectWiFi(){
    Serial.printf("\nVerbinde mit WLAN %s", ssid); // ssid ist const char*, kein String(ssid) nötig
    WiFi.begin(ssid, pass);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40)
    { // Max 20 Versuche (10 Sekunden)
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED){
        Serial.printf("\nWiFi verbunden: SSID: %s, IP-Adresse: %s\n", ssid, WiFi.localIP().toString().c_str());
        rgbLedWrite(led, 255, 0, 0);               // GRB: grün
    }
    else{
        Serial.println("\n❌ WiFi Verbindung fehlgeschlagen!");
    }
}

bool is_wlan_connected(){
  if (WiFi.status() != WL_CONNECTED) {
    if (isWlanConnected == 1) {                     // War vorher verbunden?
      Serial.println("WiFi-Verbindung verloren, reconnect...");
      rgbLedWrite(led, 0, 255, 0);                  // GRB: Rot
      isWlanConnected = 0;
    }
    connectWiFi(); 
    return false;                                   // Loop wird abgebrochen
  }
  return true;                                      // WiFi ist da, Loop darf weiterlaufen
}
