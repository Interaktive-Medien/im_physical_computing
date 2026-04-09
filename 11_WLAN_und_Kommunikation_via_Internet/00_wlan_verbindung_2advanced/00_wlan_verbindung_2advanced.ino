/***************************************************************************************************
 * WLAN-Verbindung Boilerplate
 * Wahöweise mit Hochschul-WLAN-Netzwerk (WPA2Enterprise) oder Heim-WLAN-Netzwerk (WPA2) verbinden
 * Die entsprechenden Funktionen und Passwörter sind separaten .h-Dateien ausgelagert
 * Dazu unten das passende #include-Statement aktivieren
 * Bei bestehender WLAN-Verbindung leuchtet die LED_BUILTIN auf dem Mirocontrollerboard
 ***************************************************************************************************/


#include <WiFi.h>
// #include "connectWiFi_hochschule.h"         // bei Verbindung mit Hochschulnetzwerk (zB. eduroam) aktivieren
#include "connectWiFi_zuhause.h"         // bei Verbindung mit Heimnetzwerk aktivieren

int led = LED_BUILTIN;

void setup() {
  Serial.begin(115200);
  delay(3000);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.println("Starte Verbindung...");
  connectWiFi();                 // connectWiFi() ist in connectWiFi_hochschule.h UND connectWiFi_zuhause.h. Zutreffendes oben aktivieren
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi-Verbindung verloren, reconnect...");
    connectWiFi();
    digitalWrite(led, 0);
  }
  else{
    digitalWrite(led, 1);
  }
}