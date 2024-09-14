// https://arduinogetstarted.com/tutorials/arduino-http-request?utm_content=cmp-true


#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> // by Arduino

unsigned long lastTime = 0;
unsigned long timerDelay = 15000; // 15s

const char* ssid     = "dreammakers";
const char* pass     = "dreammakers";

const char* serverName = "http://192.168.0.98"; // Server-Adresse
String serverPath = "/";  // Pfad der Anfrage

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

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
    int wert = 11;    
    
    // JSON zusammenbauen
    JSONVar myObject;
    myObject["sensor"] = sensor;
    myObject["wert"] = wert;
    myObject["api_key"] = "tPmAT5Ab3j7F9";
    String jsonString = JSON.stringify(myObject);
    // String jsonString = "{\"sensor\":\"%s\", \"wert\":%d, \"api_key\":\"tPmAT5Ab3j7F9\"}", sensor, wert;

  
    // Überprüfen, ob Wi-Fi verbunden ist
    if (WiFi.status() == WL_CONNECTED) {

      // HTTP Verbindung starten und POST-Anfrage senden
      HTTPClient http;
      String serverURL = String(serverName) + serverPath;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");
      //String httpRequestData = "sensor=value1&wert=42&api_key=apiKeyValue";   // Beispiel-Daten für den POST (ohne json-Formatierung)
      int httpResponseCode = http.POST(jsonString);

      // Prüfen der Antwort
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        Serial.println("Response: " + response);
      } else {
        Serial.printf("Error on sending POST: %d\n", httpResponseCode);
      }

      // Schließen der Verbindung
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}
