/******************************************************************************************************
 * mc.ino
 * - measure audio volume (dB) with INMP441 microphone
 * - after x seconds audio level over threshold:
 * * play a random audio track + stop it as soon as noise is over
 * * you can select the audio tracks of your choice in the db. The selection will be played randomnloy
 * * create an entry into database table with timestamp + another timestamp as soon as noise is over
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
 * audio player: RX   <->  ESP32-C6: GPIO6
 * audio player: TX   <->  ESP32-C6: GPIO7
 * basis transistor   <->  ESP32-C6: GPIO5
 *
 * Install Library "Arduino_JSON" by Arduino
 *
 * You probably need to shift the value of AUDIOVOLUME_OFFSET in get_audiovolume.h 
 * define AUDIOVOLUME_THRESHOLD, from which audio level is_heulsession should be 1
 * For testing use a smartphone decible tester like https://apps.apple.com/ch/app/dezibel-x-dba-l%C3%A4rm-messger%C3%A4t/id448155923?l=de-DE
 ******************************************************************************************************/

int ledPin = BUILTIN_LED;
#include "helper_functions.h"

////////////////////////////////////////////////////////////// WLAN + Server
#include <WiFi.h>
// #include "connectWiFi_hochschule.h"                 // activate this line when aonnecting with edu network (eg. eduroam)
#include "connectWiFi_zuhause.h"                       // activate this line when connecting with home network

////////////////////////////////////////////////////////////// audio trigger
#include "get_audiovolume.h"
#include "audioplayer.h"
#define AUDIOVOLUME_THRESHOLD 60
#define TIME_UNTIL_PLAY 2500        // this is the minimum time it should be noisy bevore audio will be triggered
      
int prev_is_heulsession = 0;
unsigned long audiotrigger_startTime = 0;
bool audio_played = false;


void setup() {
  Serial.begin(115200);
  delay(3000);
  setup_audiovolume_tester();
  initAudioPlayer();             // function is in audioplayer.h
  connectWiFi();                 // connectWiFi() is in connectWiFi_hochschule.h AND connectWiFi_zuhause.h. Activate on top
  init_audio_history_array();    // Initialize audio history array (buffer) with 0: if the audio volume > the threshold during 70% of the last x seconds --> bridging breaks. 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  Serial.println("Start WLAN connection...");
  connectWiFi();                 // connectWiFi() is in connectWiFi_hochschule.h AND connectWiFi_zuhause.h. Activate on top
  
  if (WiFi.status() == WL_CONNECTED) {
    updateSelectedTracks();      // fetch selected tracks from database. // function is in helper_functions.h
  }
  Serial.println("------------------------------------");
}


void loop(){
    float audiovolume = get_audiovolume();                       // audiotrigger
    int current_noise_detected = audiovolume > AUDIOVOLUME_THRESHOLD? 1:0;
    is_heulsession = isMostlyLoud(current_noise_detected);       // function in helper_functions() // 70% LOGIC: is_heulsession = 1 if the audio volume > the threshold during 70% of the last x seconds --> bridging breaks

    ////////////////////////////////////////////////////////////// 3 options of signal interpretation:
    ///// case 1: audio trigger just detected
    if (is_heulsession == 1 && prev_is_heulsession == 0) {
        audiotrigger_startTime = millis();                // remember start time
        audio_played = false;                             
        digitalWrite(ledPin, 1);                          // turn LED on for feedback
        // Serial.println("Heulsession detected");
    }

    ///// case 2: audio trigger has been detected already before and is still active -> play audio if mic detects loud noise long enough without interrupt
    if (is_heulsession == 1 && !audio_played) {
        if (millis() - audiotrigger_startTime >= TIME_UNTIL_PLAY) { 
            Serial.println("save audio detection in database");
            int next_track_nr = getRandomTrackId();
            // Serial.println(next_track_nr);
            playTrack(next_track_nr);                     // find this function in audioplayer.h

            String next_track_title = getRandomTrackName();
            // Serial.printf("Next track title: %s\n", next_track_title);
            audio_played = true;  
            save_into_db(is_heulsession);           
        }
    }

    ///// case 3: audio trigger is not being detected anymore.
    if (is_heulsession == 0 && prev_is_heulsession == 1) {
        digitalWrite(ledPin, 0);                           // turn LED off
        // Serial.println("Heulsession ended");
        stopTrack();                                       // find this function in audioplayer.h
        save_into_db(is_heulsession);  
    }

    prev_is_heulsession = is_heulsession;

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi-Verbindung verloren, reconnect...");
        connectWiFi();
    }
}

void save_into_db(int is_heulsession){
    JSONVar dataObject;                                      // construct JSON
    dataObject["is_heulsession"] = is_heulsession;
    dataObject["heulsession_id"] = heulsession_id;
    String jsonString = JSON.stringify(dataObject);

    if (WiFi.status() == WL_CONNECTED) {
        upload_heulsession(jsonString);                      // upload session start and stop timestamp into DB
    }
}