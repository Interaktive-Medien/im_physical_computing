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
bool isWlanConnected = 0;

void setup() {
  Serial.begin(115200);
  delay(3000);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.println("Starte Verbindung...");
  connectWiFi();                 // connectWiFi() ist in connectWiFi_hochschule.h UND connectWiFi_zuhause.h. Zutreffendes oben aktivieren
}

void loop() {
  if (!is_wlan_connected()) return; 
  // go on here
}

bool is_wlan_connected(){
  if (WiFi.status() != WL_CONNECTED) {
    if (isWlanConnected == 1) {          // War vorher verbunden?
      Serial.println("WiFi-Verbindung verloren, reconnect...");
      rgbLedWrite(led, 0, 255, 0);       // GRB: Rot
      isWlanConnected = 0;
    }
    connectWiFi(); 
    return false; // Loop wird abgebrochen
  }
  return true; // WiFi ist da, Loop darf weiterlaufen
}