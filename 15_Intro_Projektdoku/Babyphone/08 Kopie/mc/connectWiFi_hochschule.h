/**********************************************************************************************************
 * connectWiFi_hochschule.h
 * Herstellung der WLAN Verbindung mit einem WPA2-Enterprise-verschlüsselten Netzwerk.
 * Wenn zB. mit dem Hochschulnetzwerk verbunden werden soll, wird dieses Script in das Hauptscript eingebunden.
 * Für die Verbindung mit einem Heimnetzwerk connectWiFi_zuhause.h verwenden
 * Folgende auskommentierte Werte befinden sich in password_hochschule.h und werden hier direkt eingebunden
 **********************************************************************************************************/ 

// #define EAP_IDENTITY "fiessjan@fhgr.ch"            
// const char *ssid = "eduroam";                      
// const char *EAP_PASSWORD = "mypassword";           

#include "esp_eap_client.h"                           // WPA2-Enterprise API
#include "password_hochschule.h"            

void connectWiFi() {
  Serial.printf("\nVerbinde mit WLAN %s", ssid);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  // WPA2-Enterprise (EAP) Konfiguration
  esp_eap_client_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_eap_client_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_eap_client_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));

  esp_eap_client_set_disable_time_check(true);        // Zertifikats-Zeitprüfung deaktivieren (notwendig bei eduroam)
  esp_wifi_sta_enterprise_enable();                   // Aktivieren (richtige Funktion für ESP32 Arduino Core 3.3.x)

  // Verbindung starten
  WiFi.begin(ssid);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 120) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWiFi verbunden: SSID: %s, IP-Adresse: %s\n", ssid, WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n❌ Verbindung fehlgeschlagen!");
  }
}
