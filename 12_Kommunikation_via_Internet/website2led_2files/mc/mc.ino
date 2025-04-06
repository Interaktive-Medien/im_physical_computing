/***************************************************************************
 * mc.ino
 * Control LED from a website.
 * Der ESP32-C6 fungiert hier nicht als Server.
 * funktioniert nur im lokalen WLAN (oder z. B. per VPN oder port forwarding)
 * Er empfängt HTTP Nachrichten (POST-Methode) und schaltet LED entsprechend
 * JSON-formatted string is sent to <IP address of ESP3-C6>/postjson.
 * Install library "Arduino_JSON" by Arduino
 ***************************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>

int led = BUILTIN_LED;

const char* ssid = "tinkergarden";
const char* password = "strenggeheim";

WebServer server(80);

void setup() {
  Serial.begin(115200);
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

  // Route für OPTIONS-Anfragen (Preflight)
  server.on("/postjson", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204); // No content, da dies nur eine Preflight-Anfrage ist
  });

  // Route für POST-Anfragen
  server.on("/postjson", HTTP_POST, handlePostRequest);

  // Start des HTTP-Servers
  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  // Verarbeite eingehende HTTP-Anfragen
  server.handleClient();
}

// Funktion zum Verarbeiten von POST-Anfragen
void handlePostRequest() {
  // CORS-Header hinzufügen
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

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
  String response = "Received: wert = " + String(wert);
  server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"" + response + "\"}");
}
