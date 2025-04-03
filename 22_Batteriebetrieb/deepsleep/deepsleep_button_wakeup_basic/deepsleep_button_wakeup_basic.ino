/*********************************************************************************
 * deepsleep_button_wakeup_basic.ino
 * Deep sleep - wacht auf, wenn ein Button gedruckt wird.
 * Beim ESP32-C6 stehen nur GPIOs 6, 7, 8, 9, 10 und 11 für esp_sleep_enable_ext0_wakeup() zur Verfügung
 * Verbinde Button mit GPIO7 des ESP32-C6 DevKit.
 * source: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 * code from File > Examples > ESP32 Deep Sleep > ExternalWakeUp
 * Author: Pranav Cherukupalli <cherukupallip@gmail.com>
 * *******************************************************************************/

//////////////////////////////////////////////// Deep Sleep

#include "esp_sleep.h"
#include "driver/rtc_io.h"
#define BUTTON_PIN_BITMASK (1ULL << BUTTON_PIN)  
RTC_DATA_ATTR int bootCount = 0;

//////////////////////////////////////////////// Button

#define BUTTON_PIN 7  // ESP32-C6: Nur GPIOs 6-11 für ext1 Wakeup



void setup() {
  Serial.begin(115200);
  delay(1000);

  //////////////////////////////////////////////// Button
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);       // GPIO für Knopf als Input mit Pulldown setzen

  //////////////////////////////////////////////// Deep Sleep
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  // RTC IO Konfiguration für Pulldown im Deep Sleep (ansonsten braucht es einen hochohmigen Pulldown-Widerstand zwischden GPIO7 und Ground)
  rtc_gpio_init((gpio_num_t)BUTTON_PIN);
  rtc_gpio_set_direction((gpio_num_t)BUTTON_PIN, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pulldown_en((gpio_num_t)BUTTON_PIN);
  rtc_gpio_pullup_dis((gpio_num_t)BUTTON_PIN);
  rtc_gpio_hold_en((gpio_num_t)BUTTON_PIN);  // WICHTIG: Hält den Pulldown während des Deep Sleep aktiv

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);  // aufwachen, sobald ein HIGH SIgnal rein kommt (Button gedrückt)

  Serial.println("Going to sleep now until button pressed");
  delay(5000);
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop() {
  // Nicht verwendet
}
