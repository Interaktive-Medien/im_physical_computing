/******************************************************************************************
 * mc_deepsleep.ino
 * Sensordaten sammeln und als JSON-String per HTTP POST Request an Server schicken (-> load.php)
 * Zwischen den Messintervallen geht der MC in de Deep Sleep Mode. Tutorial: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 ******************************************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> // by Arduino

/* deep sleep data */
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300          /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;

/* WLAN */
const char* ssid     = "tinkergarden";
const char* pass     = "strenggeheim";

/* Sensor */
const int sensorPin = 4;

/* Server */ 
const char* serverURL = "https://fiessling.ch/iot1/06_sensor2website_diy/load.php"; 


void setup() {
  Serial.begin(115200);

  /* wakeup info from deep sleep */
  ++bootCount;   //Increment boot number and print it every reboot
  Serial.println("Boot number: " + String(bootCount));
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER){
    Serial.println("Wakeup caused by timer"); 
  }
  

  ///////////////////////////////////////////// Verbindung mit WLAN herstellen

  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());





  ///////////////////////////////////////////// tasks from loop go here


  ////////// Task 1: sensor auslesen

  float wert = analogRead(sensorPin); /* Poti, Lichtsensor, ... */
  Serial.println(wert);

  ////////// Task 2: JSON zusammenbauen

  JSONVar dataObject;
  dataObject["wert"] = wert;
  String jsonString = JSON.stringify(dataObject);

  ////////// Task 3: JSON string per HTTP POST request an den Server schicken (server2db.php)

  if (WiFi.status() == WL_CONNECTED) {   // Überprüfen, ob Wi-Fi verbunden ist
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


  /////////////////////////////////////////////// end of tasks

  /* go sleeping again */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Going to sleep for " + String(TIME_TO_SLEEP) + " Seconds");
  Serial.flush();
  esp_deep_sleep_start();
  // Serial.println("This will never be printed");
}

void loop() {}