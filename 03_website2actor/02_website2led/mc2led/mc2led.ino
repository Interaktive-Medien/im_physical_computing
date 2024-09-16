// Der HTTP-Server läuft auf Port 80 (WebServer server(80)).
// Eine Route (/postjson) wird definiert, um POST-Anfragen zu verarbeiten. 
// Die Methode server.on("/postjson", HTTP_POST, handlePostRequest) 
// weist die POST-Route der Funktion handlePostRequest() zu.

#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>

int led = BUILTIN_LED;

// Wi-Fi Zugangsdaten
const char* ssid = "dreammakers";
const char* password = "dreammakers";

// HTTP-Server läuft auf Port 80
WebServer server(80);

// Funktion zum Verarbeiten von POST-Anfragen
void handlePostRequest() {
  if (server.hasArg("plain") == false) {  // Überprüfen, ob der Body vorhanden ist
    server.send(400, "text/plain", "400: Invalid Request - No Body");
    return;
  }

  // JSON-String aus dem Body der Anfrage lesen
  String jsonstring = server.arg("plain");

  Serial.println(jsonstring);

  // JSON-Daten parsen mit Arduino_JSON
  JSONVar dataobject = JSON.parse(jsonstring);

  int wert = int(dataobject["wert"]);

  Serial.print("wert: ");
  Serial.println(wert);

  digitalWrite(led, wert);

 





}

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
