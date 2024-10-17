/******************************************************************************************
 * mc.ino
 * Sensordaten sammeln und per HTTP POST Request an Server schicken (-> load.php)
 ******************************************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> // by Arduino

unsigned long lastTime = 0;
unsigned long timerDelay = 15000; // alle 15s wird ein neuer Wert verschickt

const char* ssid     = "Igloo";
const char* pass     = "1glooVision";

// const char* serverURL = "https://192.168.0.98/06_sensor2website/server2db.php"; // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
const char* serverURL = "https://650665-4.web.fhgr.education/06_sensor2website/load.php"; 

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
