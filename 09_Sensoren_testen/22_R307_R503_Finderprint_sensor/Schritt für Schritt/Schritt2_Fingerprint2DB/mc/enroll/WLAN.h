// WLAN.h

#include <WiFi.h>

// WLAN Konfiguration
const char* ssid     = "tinkergarden";                             // WLAN SSID
const char* pass     = "strenggeheim";                             // WLAN Passwort
bool isWlanConnected = 0;

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
        rgbLedWrite(led, 255, 0, 0);               // GRB: grün
        isWlanConnected = 1;
    }
    else{
        Serial.println("\n❌ WiFi Verbindung fehlgeschlagen!");
        isWlanConnected = 0;
    }
}

bool is_wlan_connected(){
  if (WiFi.status() != WL_CONNECTED) {
    if (isWlanConnected == 1) {                     // War vorher verbunden?
      Serial.println("WiFi-Verbindung verloren, reconnect...");
      rgbLedWrite(led, 0, 255, 0);                  // GRB: Rot
      isWlanConnected = 0;
    }
    connectWiFi(); 
    return false;                                   // Loop wird abgebrochen
  }
  return true;                                      // WiFi ist da, Loop darf weiterlaufen
}