/***************************************************************************
 * Kap. 11: WLAN und Kommunikation via Internet  >  Website-to-LED (2 files - version)
 * 02_website2led_2files.ino
 * Steuere LED an diesem Mikrocontrollerboard von einer Website aus.
 * Der ESP fungiert als Server.
 * funktioniert nur im lokalen WLAN (oder z. B. per VPN oder Port Forwarding).
 * Passe ssid und pass an dein eigenes WLAN an.
 * Das Gegenstück dieses Programms (die Website) sendet einen JSON-formatierten Text an http://<MC's IP-Adresse>/postjson, wenn der HTML-Button dort gedrückt wird.
 * MC empfängt die HTTP Nachrichten (POST-Methode) und schaltet LED entsprechend.
 * Installiere Library "Arduino_JSON" by Arduino
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/11_WLAN_und_Kommunikation_via_Internet/02_website2led_2files/02_website2led_2files.ino
 ***************************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>

int led = BUILTIN_LED;

const char* ssid   = "tinkergarden";         // WLAN SSID
const char* pass   = "strenggeheim";         // WLAN password
bool isWlanConnected = 0;

WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(led, OUTPUT);
  rgbLedWrite(led, 0, 255, 0);  // GRB rot
  Serial.println("Starte Verbindung...");
  connectWiFi();

  // --- ROUTEN DEFINITIONEN ---   Browser ruft die Adresse "http://<esp's_ip_address>/postjson" auf

  // Preflight-Anfrage (Browser bittet ESP zunächst um Erlaubnis, JSON String zu schicken)
  server.on("/postjson", HTTP_OPTIONS, handlePreflightRequest);
  // Die eigentliche POST-Route
  server.on("/postjson", HTTP_POST, handlePostRequest);
    
  // Start des HTTP-Servers
  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  if (!is_wlan_connected()) return; 
  server.handleClient();
}

// Preflight-Anfrage (Browser bittet ESP zunächst um Erlaubnis, JSON String zu schicken)
void handlePreflightRequest(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204); // request succeeded, but the server intentionally returns no message body
}



// Funktion zum Verarbeiten von POST-Anfragen -> Antwort an Browser schicken
void handlePostRequest() {
  // welche Webseiten ("Origins") sind berechtigt, die Antwort dieses servers zu lesen: * -> alle
  server.sendHeader("Access-Control-Allow-Origin", "*"); 
  
  if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "400: Invalid Request - No Body");
    return;
  }

  String jsonstring = server.arg("plain");
  JSONVar dataobject = JSON.parse(jsonstring);

  int wert = dataobject["wert"];
  Serial.println(wert);
  digitalWrite(led, wert);

  String responseText = "Received: wert = " + String(wert);
  String jsonResponse = "{\"status\":\"success\",\"message\":\"hat geklappt: " + responseText + "\"}";
  server.send(200, "application/json", jsonResponse);
}

void connectWiFi(){
    Serial.printf("\nVerbinde mit WLAN %s", ssid); // ssid ist const char*, kein String(ssid) nötig
    WiFi.begin(ssid, pass);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40)
    {                                    // Max 20 Versuche (10 Sekunden)
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED){
        Serial.printf("\nWiFi verbunden: SSID: %s, IP-Adresse: %s\n", ssid, WiFi.localIP().toString().c_str());
        rgbLedWrite(led, 255, 0, 0);     // GRB: grün
    }
    else{
        Serial.println("\n❌ WiFi Verbindung fehlgeschlagen!");
        rgbLedWrite(led, 0, 255, 0);     // GRB: rot
    }
}

bool is_wlan_connected(){
  if (WiFi.status() != WL_CONNECTED) {
    if (isWlanConnected == 1) {.         // War vorher verbunden?
      Serial.println("WiFi-Verbindung verloren, reconnect...");
      rgbLedWrite(led, 0, 255, 0);       // GRB: Rot
      isWlanConnected = 0;
    }
    connectWiFi(); 
    return false; // Loop wird abgebrochen
  }
  return true; // WiFi ist da, Loop darf weiterlaufen
}
