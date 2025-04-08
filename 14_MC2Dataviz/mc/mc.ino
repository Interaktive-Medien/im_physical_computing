/******************************************************************************************
 * Kap. 14: Microcontroller -> Dataviz
 * mc.ino
 * Installiere Library "Arduino_JSON" by Arduino
 * Sensordaten sammeln und per HTTP POST Request an Server schicken (-> an load.php).
 * load.php schreibt die Werte dann in die Datenbank
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 * Gib SSID und Passwort deines WLANs an.
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/tree/main/14_MC2Dataviz/mc/mc.ino
 ******************************************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;                              // alle 15s wird ein neuer Wert verschickt

const char* ssid     = "<your_ssid>";                          // WLAN
const char* pass     = "<your_password>";                      // WLAN

const char* serverURL = "https://<your_website.ch>/im4/19_mc2website/load.php";  // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
                                                               // Beispiel: https://fiessling.ch/im4/19_mc2website/load.php
void setup() {
  Serial.begin(115200);
  

  ////////////////////////////////////////////////////////////// Verbindung mit Wi-Fi herstellen

  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());
}

void loop() {
  
  if ((millis() - lastTime) > timerDelay) {   // alle 15 Sekunden...
    lastTime = millis();


    ////////////////////////////////////////////////////////////// sensor auslesen

    float wert = (float)random(0, 1000) / 10;  // 0 - 100
    Serial.println(wert);

    
    ////////////////////////////////////////////////////////////// JSON zusammenbauen

    JSONVar dataObject;
    dataObject["wert"] = wert;
    // dataObject["wert2"] = wert2;
    String jsonString = JSON.stringify(dataObject);
    // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77}";  // stattdessen könnte man den JSON string auch so zusammenbauen

  
     ////////////////////////////////////////////////////////////// JSON string per HTTP POST request an den Server schicken (server2db.php)

    if (WiFi.status() == WL_CONNECTED) {   // Überprüfen, ob Wi-Fi verbunden ist
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
