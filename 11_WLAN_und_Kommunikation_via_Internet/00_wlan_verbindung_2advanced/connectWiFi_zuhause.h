/**********************************************************************************************************
 * connectWiFi_zuhause.h
 * Herstellung der WLAN Verbindung mit einem Heimnetzwerk.
 * Dieses Script wird in das Hauptscript eingebunden.
 * Für die Verbindung mit zB. einem Hochschulnetzwerk bitte das Script connectWiFi_hochschule.h verwenden
 * Folgende auskommentierte Werte befinden sich in password_zuhause.h und werden hier direkt eingebunden
 **********************************************************************************************************/ 

// const char *ssid = "my_ssid";            // Wert befindet sich in password_zuhause.h
// const char *pass = "my_password";        // Wert befindet sich in password_zuhause.h

#include "password_zuhause.h"            

void connectWiFi() {
  Serial.printf("\nVerbinde mit WLAN %s", ssid);            // ssid ist const char*, kein String(ssid) nötig
  WiFi.begin(ssid, pass);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {  // Max 20 Versuche (10 Sekunden)
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWiFi verbunden: SSID: %s, IP-Adresse: %s\n", ssid, WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n❌ WiFi Verbindung fehlgeschlagen!");
  }
}