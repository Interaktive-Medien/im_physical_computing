/*********************************************************************************
 * deepsleep_button_wakeup_basic.ino
 * Deep sleep mit Timer - wacht auf, wenn ein Button gedruckt wird.
 * Beim ESP32-C6 stehen nur GPIOs 6, 7, 8, 9, 10 und 11 für esp_sleep_enable_ext0_wakeup() zur Verfügung
 * Verbinde Button Input mit GPIO7.
 * Es muss nichts an den ESP32 angeschlossen werden
 * source: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 * code from File > Examples > ESP32 Deep Sleep > ExternalWakeUp
 * Author: Pranav Cherukupalli <cherukupallip@gmail.com>
 * *******************************************************************************/

//////////////////////////////////////////////// Deep Sleep

#include "driver/rtc_io.h"
#define BUTTON_PIN_BITMASK (1ULL << BUTTON_PIN)  
RTC_DATA_ATTR int bootCount = 0;

//////////////////////////////////////////////// Button

#define BUTTON_PIN 7  // ESP32-C6: Nur GPIOs 6-11 für ext1 Wakeup


//////////////////////////////////////////////// Deep Sleep

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  //////////////////////////////////////////////// Button

  // GPIO für Knopf als Input mit Pulldown setzen
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);


  //////////////////////////////////////////////// Deep Sleep

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();

  // RTC IO Konfiguration für Pulldown im Deep Sleep (ansonsten braucht es einen hochohmigen Widerstand zwischden GPIO7 und Ground)
  rtc_gpio_init((gpio_num_t)BUTTON_PIN);
  rtc_gpio_set_direction((gpio_num_t)BUTTON_PIN, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pulldown_en((gpio_num_t)BUTTON_PIN);
  rtc_gpio_pullup_dis((gpio_num_t)BUTTON_PIN);
  rtc_gpio_hold_en((gpio_num_t)BUTTON_PIN);  // WICHTIG: Hält den Pulldown während des Deep Sleep aktiv


  //////////////////////////////////////////////// Deep Sleep

  // Wakeup auf HIGH-Signal (wenn der Button gedrückt wird)
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
  // Nicht verwendet
}
