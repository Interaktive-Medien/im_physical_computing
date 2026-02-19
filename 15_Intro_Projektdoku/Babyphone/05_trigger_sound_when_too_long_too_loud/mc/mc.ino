/******************************************************************************************************
 * mc.ino
 * measure audio volume (dB) with INMP441 microphone and play an audio track if too loud for some time.
 *
 * microphone INMP441: Connect ...
 * INMP441: VDD  <->  ESP32-C6: 3.3V 
 * INMP441: GND  <->  ESP32-C6: GND 
 * INMP441: SD   <->  ESP32-C6: GPIO 13 
 * INMP441: SCK  <->  ESP32-C6: GPIO 2 
 * INMP441: WS   <->  ESP32-C6: GPIO 23 
 * INMP441: L/R  <->  ESP32-C6: GND 
 *
 * Audio player: Connect ...
 * audio player: Vin  <->  ESP32-C6: 5V 
 * audio player: GND  <->  ESP32-C6: GND  (per npn transistor)
 * audio player: RX   <->  ESP32-C6: GPIO7
 * audio player: TX   <->  ESP32-C6: GPIO6
 * basis transistor   <->  ESP32-C6: GPIO5
 *
 * You probably need to shift the value of AUDIOVOLUME_OFFSET in get_audiovolume.h(adjust the decible threshold from when it should trigger). 
 * define AUDIOVOLUME_THRESHOLD, from which loudness on the triggerstate should me 1
 * For testing use a smartphone decible tester like https://apps.apple.com/ch/app/dezibel-x-dba-l%C3%A4rm-messger%C3%A4t/id448155923?l=de-DE
 ******************************************************************************************************/


#include "get_audiovolume.h"
#include "audioplayer.h"
#define AUDIOVOLUME_THRESHOLD 60
#define TIME_UNTIL_PLAY 2000

int ledPin = BUILTIN_LED;


// button
int triggerState = 0;         
int prev_triggerState = 0;


unsigned long pressStartTime = 0;
bool audio_played = false;



void setup() {
  Serial.begin(115200);
  setup_audiovolume_tester();
  initAudioPlayer();

  // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
}

void loop(){
    float audiovolume = get_audiovolume();
    Serial.println("Min:20,Max:80,Audio_Volume:");
    Serial.println(audiovolume); 
    
    if(audiovolume > AUDIOVOLUME_THRESHOLD){
        triggerState = 1;
    }
    else{
        triggerState = 0;
    }



    // case 1: audio trigger just detected just pressed
    if (triggerState == 1 && prev_triggerState == 0) {
        pressStartTime = millis();         // remember start time
        audio_played = false;              // Reset für diesen Tastendruck
        digitalWrite(ledPin, 1);           // turn LED on for feedback
    }

    // case 2: audio trigger has been detected already before and is still active -> play audio if mic detects loud noise long enough without interrupt
    if (triggerState == 1 && !audio_played) {
        if (millis() - pressStartTime >= TIME_UNTIL_PLAY) { 
        Serial.println("fire");
        playTrack(1);
        audio_played = true;             
        }
    }

    // case 3: audio trigger is not being detected anymore.
    if (triggerState == 0 && prev_triggerState == 1) {
        digitalWrite(ledPin, 0);             // LED off
    }

    prev_triggerState = triggerState;
}