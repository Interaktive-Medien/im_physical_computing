/*********************************************************************************
 * deepsleep_timer_wakeup_basic.ino
 * Deep sleep mit Timer - wacht alle 5s auf
 * Es muss nichts an den ESP32 angeschlossen werden
 * source: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 * code adapted from File > Examples > ESP32 Deep Sleep > TimerWakeUp
 * Author: Pranav Cherukupalli <cherukupallip@gmail.com>
 * *******************************************************************************/

//////////////////////////////////////////////// Deep Sleep
#include "esp_sleep.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;  /* Um den Zähler im Speicher (8kB - wird im Deep Sleep nicht gelöscht) zu speichern, muss "RTC_DATA_ATTR" davor gechrieben werden*/

void setup(){
  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor

  //////////////////////////////////////////////// Deep Sleep: Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);   // alle 5s aufwachen

  Serial.println("Going to sleep now for 5s");
  delay(5000);
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop(){
  // Nicht verwendet
}