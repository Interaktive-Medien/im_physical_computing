#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> // by Arduino

unsigned long lastTime = 0;
unsigned long timerDelay = 15000; // 15s

const char* ssid     = "dreammakers";
const char* pass     = "dreammakers";

const char* serverName = "https://192.168.0.98"; // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
String serverPath = "/05_mc2db/server2db.php";  // Pfad der Anfrage

// Keep this API Key value to be compatible with the PHP code server2db.php. That file also needs to have the same key 
String apiKeyValue = "im";

void setup() {
  Serial.begin(115200);
  
  // Verbindung mit Wi-Fi herstellen
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());
}

void loop() {
  // alle 10 Sekunden
  if ((millis() - lastTime) > timerDelay) {
    lastTime = millis();

    // sensor auslesen
    String sensor = "mysensor";
    float wert1 = (float)random(0, 1000) / 10;  // 0 - 100  
    float wert2 = (float)random(0, 1000) / 10;  // 0 - 100
    
    // JSON zusammenbauen
    JSONVar dataObject;
    dataObject["sensor"] = sensor;
    dataObject["wert1"] = wert1;
    dataObject["wert2"] = wert2;
    dataObject["api_key"] = apiKeyValue;
    String jsonString = JSON.stringify(dataObject);
    // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77, \"api_key\":\"im\"}";

    // Überprüfen, ob Wi-Fi verbunden ist
    if (WiFi.status() == WL_CONNECTED) {

      // HTTP Verbindung starten und POST-Anfrage senden
      HTTPClient http;
      String serverURL = String(serverName) + serverPath;
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