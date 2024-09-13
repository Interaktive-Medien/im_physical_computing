// https://arduinogetstarted.com/tutorials/arduino-http-request?utm_content=cmp-true


#include <WiFi.h>
#include <HTTPClient.h>

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
  // Sendet alle 10 Sekunden eine POST-Anfrage
  if ((millis() - lastTime) > timerDelay) {
    // Zeit aktualisieren
    lastTime = millis();

    // Überprüfen, ob Wi-Fi verbunden ist
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Server-URL und Pfad
      String serverURL = String(serverName) + serverPath;

      // Begin der HTTP-Verbindung
      http.begin(serverURL);

      // Definieren des POST-Typs und Inhalts
      http.addHeader("Content-Type", "application/json");

      // Erstellen des JSON-Strings
      String jsonData = "{\"sensor\":\"sensor1\", \"wert\":43, \"api_key\":\"tPmAT5Ab3j7F9\"}";

      // Beispiel-Daten für den POST
      //String httpRequestData = "sensor=value1&wert=42&api_key=apiKeyValue";

      // Senden der POST-Anfrage
      int httpResponseCode = http.POST(jsonData);

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
