#include "esp_sleep.h"
RTC_DATA_ATTR int bootCount = 0;

#define REED_SWITCH_GPIO 7  // Anpassen an deinen GPIO

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(REED_SWITCH_GPIO, INPUT_PULLDOWN); // Aktiviert internen Pull-down
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  esp_sleep_enable_ext1_wakeup(1ULL << REED_SWITCH_GPIO, ESP_EXT1_WAKEUP_ANY_LOW); // Wakeup bei fallender Flanke
  Serial.println("Going to sleep now until button pressed");
  delay(1000);
  esp_deep_sleep_start(); // Deep Sleep aktivieren
}

void loop() {
  // Der Code hier wird nach dem Aufwachen ausgeführt
}