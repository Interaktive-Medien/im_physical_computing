/******************************************************************************************************
 * Kap. 11: WLAN und Kommunikation via Internet  >  Website-to-LED (LittleFS)
 * ESP32-C6 fungiert als Webserver und liefert HTML/CSS/JS direkt aus dem Dateisystem aus.
 ******************************************************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include "FS.h"
#include "LittleFS.h"

const char* ssid     = "tinkergarden";     // WLAN SSID
const char* pass     = "strenggeheim";     // WLAN Passwort
bool isWlanConnected = false;

// Webserver auf Port 80 instanziieren
WebServer server(80);

const int led = BUILTIN_LED;

// Funktion, um Dateien aus dem LittleFS an den Browser zu senden
bool handleFileRead(String path) {
  Serial.println("Erhalte Request für: " + path);
  
  // Wenn der Pfad auf "/" endet, lade die index.html
  if (path.endsWith("/")) {
    path += "index.html";
  }

  // Bestimme den Content-Type anhand der Dateiendung
  String contentType = "text/plain";
  if (path.endsWith(".html")) contentType = "text/html";
  else if (path.endsWith(".css"))  contentType = "text/css";
  else if (path.endsWith(".js"))   contentType = "application/javascript";
  else if (path.endsWith(".ico"))  contentType = "image/x-icon";

  // Prüfen, ob die Datei im LittleFS existiert
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    // Datei direkt an den Client streamen
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  
  Serial.println("⚠️ Datei nicht gefunden: " + path);
  return false;
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  pinMode(led, OUTPUT);
  rgbLedWrite(led, 0, 255, 0);  // Status: Rot (Verbindet)

  // 1. LittleFS initialisieren
  if (!LittleFS.begin(true)) {
    Serial.println("❌ LittleFS Mount fehlgeschlagen!");
    return;
  }
  Serial.println("Information: LittleFS erfolgreich geladen.");

  // 2. WLAN starten
  Serial.println("Starte Verbindung...");
  connectWiFi();

  // 3. HTTP-Routen definieren

  // API-Endpunkt für das JavaScript zum Schalten der LED
  server.on("/setLED", HTTP_GET, []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      if (state == "on") {
        digitalWrite(led, HIGH);
        server.send(200, "text/plain", "LED eingeschaltet");
      } else if (state == "off") {
        digitalWrite(led, LOW);
        server.send(200, "text/plain", "LED ausgeschaltet");
      }
    } else {
      server.send(400, "text/plain", "Bad Request: Parameter fehlt");
    }
  });

  // Universal-Handler: Fängt alle Anfragen ab, die keine API-Routen sind (z.B. /, /style.css, /script.js)
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "404: File Not Found");
    }
  });

  // Webserver starten
  server.begin();
  Serial.println("HTTP Webserver gestartet.");
}

void loop() {
  // WLAN Reconnect-Logik
  if (WiFi.status() != WL_CONNECTED) {
    if (isWlanConnected) {
      Serial.println("WiFi-Verbindung verloren, reconnect...");
      rgbLedWrite(led, 0, 255, 0);  // Rot
      isWlanConnected = false;
    }
    connectWiFi();
  } else {
    if (!isWlanConnected) {
      rgbLedWrite(led, 255, 0, 0); // Grün bei Erfolg
      isWlanConnected = true;
    }
  }

  // Wichtig: Eingehende Webserver-Anfragen verarbeiten
  server.handleClient();
}

void connectWiFi() {
  Serial.printf("\nVerbinde mit WLAN %s", ssid);
  WiFi.begin(ssid, pass);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWiFi verbunden! IP-Adresse: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n❌ WiFi Verbindung fehlgeschlagen!");
  }
}