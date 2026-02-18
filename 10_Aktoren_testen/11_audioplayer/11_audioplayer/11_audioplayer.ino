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