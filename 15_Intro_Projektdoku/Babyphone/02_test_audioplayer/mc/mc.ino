/******************************************************************************************************
 * mc.ino
 * control Open-Smart audio player with ESP32-C6
 * Connect ...
 * audio player: Vin  <->  ESP32-C6: 5V 
 * audio player: GND  <->  ESP32-C6: GND  (per npn transistor)
 * audio player: RX   <->  ESP32-C6: GPIO7
 * audio player: TX   <->  ESP32-C6: GPIO6
 * basis transistor  <->   ESP32-C6: GPIO5
 ******************************************************************************************************/



#include "audioplayer.h"

void setup() {
  Serial.begin(115200);
  initAudioPlayer();
}

void loop() {
  // Loop durch Tracks 1 bis 10
  for (int i = 1; i <= 10; i++) {
    Serial.print("Spiele Track: ");
    Serial.println(i);
    playTrack(i);
    delay(6000); 
  }
  
  Serial.println("Alle 10 Songs durch, fange von vorne an...");
}