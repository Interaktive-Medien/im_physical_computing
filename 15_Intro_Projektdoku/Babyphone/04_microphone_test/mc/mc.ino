/******************************************************************************************************
 * 21_INMP441_Microphone.ino
 * measure audio volume (dB) with INMP441 microphone and turn on light if too loud.
 * Connect ...
 * INMP441: VDD  <->  ESP32-C6: 3.3V 
 * INMP441: GND  <->  ESP32-C6: GND 
 * INMP441: SD   <->  ESP32-C6: GPIO 13 
 * INMP441: SCK  <->  ESP32-C6: GPIO 2 
 * INMP441: WS   <->  ESP32-C6: GPIO 23 
 * INMP441: L/R  <->  ESP32-C6: GND 
 * You probably need to shift the value of DB_OFFSET. 
 * For testing use a smartphone decible tester like https://apps.apple.com/ch/app/dezibel-x-dba-l%C3%A4rm-messger%C3%A4t/id448155923?l=de-DE
 ******************************************************************************************************/


#include "get_audiovolume.h"

int ledPin = BUILTIN_LED;

void setup() {
  Serial.begin(115200);
  setup_audiovolume_tester();

  // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
}

void loop(){
    float audiovolume = get_audiovolume();
    Serial.println("Min:20,Max:80,Audio_Volume:");
    Serial.println(audiovolume); 
    
    if(audiovolume > 75){
        digitalWrite(ledPin, 1);
    }
    else{
        digitalWrite(ledPin, 0);
    }
}