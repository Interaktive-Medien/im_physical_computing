/*********************************************************************************
 * Kap. 17: Batteriebetrieb > Deep Sleep > Timer Wakeup Value -> DB
 * deepsleep_timer_wakeup_value2db.ino
 * Wache alle 5s auf und speichere einen Sensorwert in einer Datenbank
 * (verbinde mit dem WLAN und sende den Wert an load.php)
 * Es muss nichts an den ESP32 angeschlossen werden
 * Mehr Infos zum Deep Sleep Modus: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 * Installiere Library "Arduino_JSON" by Arduino, um JSON Strings für load.php einfacher zu kreieren.
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 * Gib SSID und Passwort deines WLANs an.
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/17_Batteriebetrieb/deepsleep/deepsleep_timer_wakeup_basic/deepsleep_timer_wakeup_basic.ino
 * *******************************************************************************/



//////////////////////////////////////////////// Deep Sleep

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  1200        /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;  /* Um den Zähler im Speicher (8kB - wird im Deep Sleep nicht gelöscht) zu speichern, muss "RTC_DATA_ATTR" davor gechrieben werden*/

//////////////////////////////////////////////// Web-Kommunikation

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 

const char* ssid      = "LinusFetzMusikGast";                          // WLAN
const char* pass      = "linusfetzgast";                      // WLAN
const char* serverURL = "https://fiessling.ch/physco/load.php";  // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
                                            // Beispiel: https://fiessling.ch/im4/18_mc2db/load.php
    
void setup(){
  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor


  //////////////////////////////////////////////// Deep Sleep

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));


  //////////////////////////////////////////////// sensor auslesen

  float wert = (float)random(0, 1000) / 10;  // 0 - 100
  Serial.println(wert);  


  ////////////////////////////////////////////////////////////// JSON zusammenbauen

  JSONVar dataObject;
  dataObject["wert"] = wert;
  String jsonString = JSON.stringify(dataObject);
  // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77}";  // stattdessen könnte man den JSON string auch so zusammenbauen


  //////////////////////////////////////////////// Web-Kommunikation: WLAN-Verbundung herstellen

  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());


  if (WiFi.status() == WL_CONNECTED) {                        // Überprüfen, ob Wi-Fi verbunden ist
      // HTTP Verbindung starten und POST-Anfrage senden
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonString);

    // Prüfen der Antwort
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      Serial.println("Response: " + response);
    } else {
      Serial.printf("Error on sending POST: %d\n", httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

    

  //////////////////////////////////////////////// Deep Sleep

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop(){
  //This is not going to be called
}