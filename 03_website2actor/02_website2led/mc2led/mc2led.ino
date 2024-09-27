#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>

int led = BUILTIN_LED;

// Wi-Fi Zugangsdaten
const char* ssid = "Igloo";
const char* password = "1glooVision";

// HTTP-Server auf Port 80
WebServer server(80);



// Setup-Funktion
void setup() {
  // Start der seriellen Kommunikation
  Serial.begin(115200);

  // init LED
  pinMode(led, OUTPUT);

  // Verbindung zum WLAN herstellen
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi!");

  // IP-Adresse des ESP32 anzeigen
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Route für POST-Anfragen
  server.on("/postjson", HTTP_POST, handlePostRequest);

  // Start des HTTP-Servers
  server.begin();
  Serial.println("HTTP server started.");
}

// Loop-Funktion
void loop() {
  // Verarbeite eingehende HTTP-Anfragen
  server.handleClient();
}


// Funktion zum Verarbeiten von POST-Anfragen
void handlePostRequest() {
  // CORS-Header hinzufügen
  // Route für OPTIONS-Anfragen (Preflight)
  server.on("/postjson", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204); // No content, da dies nur eine Preflight-Anfrage ist
  });
  
  if (server.hasArg("plain") == false) {  // Überprüfen, ob der Body vorhanden ist
    server.send(400, "text/plain", "400: Invalid Request - No Body");
    return;
  }

  // JSON-String aus dem Body der Anfrage lesen
  String jsonstring = server.arg("plain");

  // JSON-Daten parsen mit Arduino_JSON
  JSONVar dataobject = JSON.parse(jsonstring);

  // Prüfen, ob das Parsen erfolgreich war
  if (JSON.typeof(dataobject) == "undefined") {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON format\"}");
    return;
  }
  int wert = dataobject["wert"];
  Serial.println(wert);
  digitalWrite(led, wert);

  // Antwort zurück an den Client senden
  String response = "Received: wert = " + wert;
  server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"" + response + "\"}");
}
