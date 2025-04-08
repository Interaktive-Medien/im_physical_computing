/*********************************************************************************
 * deepsleep_button_wakeup_value2db.ino
 * Vermerke in der Datenbank, wann immer der Button gedrückt wird 
 * -> per HTTP POST Request an Server schicken (-> load.php). load.php schreibt die Werte dann in die Datenbank
 * Ansonsten spare Strom und schicke den ESP32-C6 in den Deep Sleep Modus. Durch Knopfdruck wacht er wieder auf.
 * Beim ESP32-C6 stehen nur GPIOs 6, 7, 8, 9, 10 und 11 für esp_sleep_enable_ext0_wakeup() zur Verfügung
 * Verbinde Button Input mit GPIO7.
 * Mehr Infos zum Deep Sleep Modus: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 * Installiere Library "Arduino_JSON" by Arduino, um JSON Strings für load.php einfacher zu kreieren.
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 * Gib SSID und Passwort deines WLANs an.
 * *******************************************************************************/

//////////////////////////////////////////////// Deep Sleep

#include "driver/rtc_io.h"
#define BUTTON_PIN_BITMASK (1ULL << BUTTON_PIN)  
RTC_DATA_ATTR int bootCount = 0;

//////////////////////////////////////////////// Button

#define BUTTON_PIN 7  // ESP32-C6: Nur GPIOs 6-11 für ext1 Wakeup

//////////////////////////////////////////////// Web-Kommunikation

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 

const char* ssid      = "<your_ssid>";                          // WLAN
const char* pass      = "<your_password>";                      // WLAN
const char* serverURL = "https://<your_website.ch>/im4/18_mc2db/load.php";  // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
                                            // Beispiel: https://fiessling.ch/im4/18_mc2db/load.php
    

void setup() {
  Serial.begin(115200);
  delay(1000);

  //////////////////////////////////////////////// Button

  // GPIO für Knopf als Input mit Pulldown setzen
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);


  //////////////////////////////////////////////// Deep Sleep

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  // RTC IO Konfiguration für Pulldown im Deep Sleep (ansonsten braucht es einen hochohmigen Widerstand zwischden GPIO7 und Ground)
  rtc_gpio_init((gpio_num_t)BUTTON_PIN);
  rtc_gpio_set_direction((gpio_num_t)BUTTON_PIN, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pulldown_en((gpio_num_t)BUTTON_PIN);
  rtc_gpio_pullup_dis((gpio_num_t)BUTTON_PIN);
  rtc_gpio_hold_en((gpio_num_t)BUTTON_PIN);  // WICHTIG: Hält den Pulldown während des Deep Sleep aktiv




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

  // Wakeup auf HIGH-Signal (wenn der Button gedrückt wird)
  
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  Serial.println("Going to sleep now");
  Serial.flush();
  delay(1000);
  esp_deep_sleep_start();
}

void loop() {
  // Nicht verwendet
}
