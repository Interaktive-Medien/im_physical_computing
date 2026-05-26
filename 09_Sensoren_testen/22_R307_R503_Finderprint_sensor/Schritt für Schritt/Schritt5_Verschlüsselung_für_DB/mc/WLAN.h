// WLAN.h

#include <WiFi.h>

// WLAN Konfiguration
const char* ssid     = "tinkergarden";                             // WLAN SSID
const char* pass     = "strenggeheim";                             // WLAN Passwort
bool isWlanConnected = 0;

// Setzt die LED basierend auf dem aktuellen WLAN-Status (Grün bei OK, Rot bei Fehler)
void resetStatusLed() {
  if (WiFi.status() == WL_CONNECTED) {
    rgbLedWrite(led, 50, 0, 0);               // GRB: Grün
  } else {
    rgbLedWrite(led, 0, 50, 0);               // GRB: Rot
  }
}

void connectWiFi(){
    Serial.printf("\nVerbinde mit WLAN %s", ssid); 
    WiFi.begin(ssid, pass);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40)
    { // Max 20 Versuche (10 Sekunden)
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED){
        Serial.printf("\nWiFi verbunden: SSID: %s, IP-Adresse: %s\n", ssid, WiFi.localIP().toString().c_str());
        isWlanConnected = 1;
    }
    else{
        Serial.println("\nWiFi Verbindung fehlgeschlagen!");
        isWlanConnected = 0;
    }
    resetStatusLed(); // LED entsprechend setzen
}

bool is_wlan_connected(){
  if (WiFi.status() != WL_CONNECTED) {
    if (isWlanConnected == 1) {                     // War vorher verbunden?
      Serial.println("WiFi-Verbindung verloren, reconnect...");
      isWlanConnected = 0;
    }
    connectWiFi(); 
    return false;                                   // Loop wird abgebrochen
  }
  return true;                                      // WiFi ist da, Loop darf weiterlaufen
}