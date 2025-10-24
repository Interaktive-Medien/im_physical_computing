
#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_wpa2.h"  // WPA2 Enterprise-Bibliothek für eduroam
#include "password.h"

// eduroam-Zugangsdaten (ANPASSEN!)
#define EAP_IDENTITY "fiessjan@fhgr.ch"  // Dein Benutzername mit Realm
// #define EAP_PASSWORD ""                  // Dein eduroam-Passwort
const char *ssid = "eduroam";            // SSID bleibt "eduroam"
int led = LED_BUILTIN;


void setup() {
  Serial.begin(115200);
  delay(3000);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.println("huhu");

  // eduroam WiFi verbinden
  connectWiFi();
}


void loop() {
  // WiFi-Verbindung prüfen
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi-Verbindung verloren! Reconnect...");
    connectWiFi();
  }
}


void connectWiFi() {
  Serial.println("\nVerbinde mit eduroam...");

  // WiFi trennen und neu konfigurieren
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  // WPA2 Enterprise-Konfiguration für eduroam
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wifi_sta_wpa2_ent_enable();

  // Mit eduroam verbinden
  WiFi.begin(ssid);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 120) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi verbunden!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(led,1);

    delay(2000);
  } else {
    Serial.println("\nWiFi-Verbindung fehlgeschlagen!");
    digitalWrite(led,0);
    delay(3000);
  }
}




