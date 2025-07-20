
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  20 * 60             // 20 Minuten
#define MAX_WIFI_CONNECT_TIME_MS 10000     // 10 Sekunden WLAN-Verbindungsversuch
#define MAX_BOOTCOUNT_BEFORE_RESET 100     // optionaler Reset nach vielen Boot-Zyklen

RTC_DATA_ATTR int bootCount = 0;   // bleibt über Deep Sleep hinweg erhalten

const char* ssid      = "LinusFetzMusikGast";
const char* pass      = "linusfetzgast";
const char* serverURL = "https://fiessling.ch/physco/load4.php";

void goToSleep(int sleeptime) {
  Serial.println("→ Gehe jetzt in Deep Sleep.");
  esp_sleep_enable_timer_wakeup(sleeptime * uS_TO_S_FACTOR);
  Serial.flush();
  esp_deep_sleep_start();
}

bool connectToWiFi() {
  Serial.print("WLAN verbinden ");
  WiFi.begin(ssid, pass);

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < MAX_WIFI_CONNECT_TIME_MS) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWLAN verbunden. IP: %s\n", WiFi.localIP().toString().c_str());
    return true;
  } else {
    Serial.println("\nWLAN-Verbindung fehlgeschlagen.");
    return false;
  }
}

void sendDataToServer(float wert) {
  HTTPClient http;
  http.setTimeout(5000); // 5s Timeout für HTTP

  JSONVar data;
  data["wert"] = wert;
  String jsonString = JSON.stringify(data);

  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.printf("HTTP-Code: %d\nAntwort vom Empfänger: %s\n", httpResponseCode, response.c_str());
  } else {
    Serial.printf("HTTP-POST fehlgeschlagen. Fehlercode: %d\n", httpResponseCode);
  }

  http.end();
}

void setup() {
  Serial.begin(115200);
  delay(500); // etwas Zeit zum Start

  // Boot-Count hochzählen
  bootCount++;
  Serial.printf("\n\n Boot #%d\n", bootCount);

  // Optionaler Reset nach vielen Wiederholungen
  if (bootCount >= MAX_BOOTCOUNT_BEFORE_RESET) {
    Serial.println("⚠️ Zu viele Boots. Reset...");
    ESP.restart();
  }

  // Beispiel: Zufallswert erzeugen (Sensorwert)
  // float wert = (float)random(0, 1000) / 10.0;
  // Serial.printf("📈 Zufallswert: %.1f\n", wert);
  int wert = float(bootCount);

  // Mit WLAN verbinden
  if (connectToWiFi()) {
    sendDataToServer(wert);
    goToSleep(TIME_TO_SLEEP);
  }
  else{
    ESP.restart();
  }
}

void loop() {
  // wird nie aufgerufen
}
