/***************************************************************************
 * Kap. 11: WLAN und Kommunikation via Internet  >  Website-to-LED (2 files - version)
 * 02_website2led_2files.ino
 * Steuere LED an diesem MC von einer Website aus.
 * Der ESP32-C6 fungiert hier nicht als Server.
 * funktioniert nur im lokalen WLAN (oder z. B. per VPN oder Port Forwarding).
 * Passe ssid und pass an dein eigenes WLAN an.
 * Das Gegenstück dieses Peogramms (die Website) sendet einen JSON-formatierten Text an <MC's IP-Adresse>/postjson, wenn der HTML-Button dort gedrückt wird.
 * MC empfängt die HTTP Nachrichten (POST-Methode) und schaltet LED entsprechend.
 * Installiere Library "Arduino_JSON" by Arduino
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/11_WLAN_und_Kommunikation_via_Internet/02_website2led_2files/02_website2led_2files.ino
 ***************************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>

int led = BUILTIN_LED;

const char* ssid     = "tinkergarden";                          // WLAN
const char* password     = "strenggeheim";                      // WLAN

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

  // Start des HTTP-Servers
  server.begin();
  // Route für POST-Anfragen
  server.on("/postjson", HTTP_POST, handlePostRequest);
  Serial.println("HTTP server started.");
}

void loop() {
  server.handleClient();
}

// Funktion zum Verarbeiten von POST-Anfragen
void handlePostRequest() {
  if (server.hasArg("plain") == false) {  // Überprüfen, ob der Body vorhanden ist
    server.send(400, "text/plain", "400: Invalid Request - No Body");
    return;
  }

  // JSON-String aus dem Body der Anfrage lesen
  String jsonstring = server.arg("plain");

  // JSON-Daten parsen mit Arduino_JSON
  JSONVar dataobject = JSON.parse(jsonstring);

  int wert = dataobject["wert"];
  Serial.println(wert);
  digitalWrite(led, wert);

  // Antwort zurück an den Client senden
  String response = "Received: wert = " + String(wert);
  server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"" + response + "\"}");
}
