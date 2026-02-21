/******************************************************************************************************
 * mc.ino
 * - measure audio volume (dB) with INMP441 microphone
 * - after x seconds audio level over threshold:
 * * play an audio track + stop it as soon as noise is over
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
 * audio player: RX   <->  ESP32-C6: GPIO7
 * audio player: TX   <->  ESP32-C6: GPIO6
 * basis transistor   <->  ESP32-C6: GPIO5
 *
 * Install Library "Arduino_JSON" by Arduino
 *
 * You probably need to shift the value of AUDIOVOLUME_OFFSET in get_audiovolume.h 
 * define AUDIOVOLUME_THRESHOLD, from which audio level is_heulsession should be 1
 * For testing use a smartphone decible tester like https://apps.apple.com/ch/app/dezibel-x-dba-l%C3%A4rm-messger%C3%A4t/id448155923?l=de-DE
 ******************************************************************************************************/

int ledPin = BUILTIN_LED;

////////////////////////////////////////////////////////////// WLAN + Server
#include <WiFi.h>
#include <HTTPClient.h>
// #include "connectWiFi_hochschule.h"                 // activate this line when aonnecting with edu network (eg. eduroam)
#include "connectWiFi_zuhause.h"                       // activate this line when connecting with home network
const char* serverURL = "https://heulradar.hausmaenner.ch/db/load.php";
#include <Arduino_JSON.h> 
int heulsession_id = 0;                                // entry id from database table will be stored here


////////////////////////////////////////////////////////////// audio trigger
#include "get_audiovolume.h"
#include "audioplayer.h"
#define AUDIOVOLUME_THRESHOLD 60
#define TIME_UNTIL_PLAY 2500
int is_heulsession = 0;         
int prev_is_heulsession = 0;
unsigned long audiotrigger_startTime = 0;
bool audio_played = false;

///// smooting audio: if the audio level of x% from the rexent x seconds were above the threshold audio level, then is_heulsession is 1

#define BUFFER_SIZE_SMOOTH 25                    // check for audio volume every 100ms -> 25 Werte for 2.5s
int heul_history[BUFFER_SIZE_SMOOTH];
int history_index = 0;
unsigned long last_history_update = 0;

void setup() {
  Serial.begin(115200);
  delay(3000);
  setup_audiovolume_tester();
  initAudioPlayer();             // function is in audioplayer.h
  connectWiFi();                 // connectWiFi() is in connectWiFi_hochschule.h AND connectWiFi_zuhause.h. Activate on top

  // Initialize history array with 0
  for(int i = 0; i < BUFFER_SIZE_SMOOTH; i++) {
    heul_history[i] = 0;
  }

  ////////////////////////////////////////////////////////////// LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);

  ////////////////////////////////////////////////////////////// WLAN
  Serial.println("Start connection...");
  connectWiFi();                 // connectWiFi() is in connectWiFi_hochschule.h AND connectWiFi_zuhause.h. Activate on top
}

void loop(){

    ////////////////////////////////////////////////////////////// audiotrigger
    float audiovolume = get_audiovolume();
    
    // NEW: temporary variable to store instantaneous noise detection
    int current_noise_detected = 0;
    if(audiovolume > AUDIOVOLUME_THRESHOLD){
        current_noise_detected = 1;
    }

    // 70% LOGIC: is_heulsession = 1 if the audio volume > the threshold during 70% of the last x seconds --> bridging breaks
    if (millis() - last_history_update >= 100) { // update every 100ms
        last_history_update = millis();
        heul_history[history_index] = current_noise_detected; // store instantaneous value
        history_index = (history_index + 1) % BUFFER_SIZE_SMOOTH;

        int count_ones = 0;
        for (int i = 0; i < BUFFER_SIZE_SMOOTH; i++) {
            if (heul_history[i] == 1) count_ones++;
        }

        // noise during 70% of the time (18 of 25 values):
        // Only update is_heulsession here to prevent flickering
        if (count_ones >= (BUFFER_SIZE_SMOOTH * 0.7)) {
            is_heulsession = 1;
        } else {
            is_heulsession = 0;
        }
    }

    ////////////////////////////////////////////////////////////// 3 options of signal interpretation:
    ///// case 1: audio trigger just detected
    if (is_heulsession == 1 && prev_is_heulsession == 0) {
        audiotrigger_startTime = millis();                // remember start time
        audio_played = false;                             
        digitalWrite(ledPin, 1);                          // turn LED on for feedback
        Serial.println("Heulsession detected (stabilized)");
    }

    ///// case 2: audio trigger has been detected already before and is still active -> play audio if mic detects loud noise long enough without interrupt
    if (is_heulsession == 1 && !audio_played) {
        if (millis() - audiotrigger_startTime >= TIME_UNTIL_PLAY) { 
            Serial.println("fire");
            playTrack(1);
            audio_played = true;  
            save_into_db(is_heulsession);           
        }
    }

    ///// case 3: audio trigger is not being detected anymore.
    if (is_heulsession == 0 && prev_is_heulsession == 1) {
        digitalWrite(ledPin, 0);                            // turn LED off
        Serial.println("Heulsession ended");
        save_into_db(is_heulsession);  
    }

    prev_is_heulsession = is_heulsession;

    ////////////////////////////////////////////////////////////// WLAN
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi-Verbindung verloren, reconnect...");
        connectWiFi();
    }
}

void save_into_db(int is_heulsession){

    ////////////////////////////////////////////////////////////// construct JSON 

    JSONVar dataObject;
    dataObject["is_heulsession"] = is_heulsession;
    dataObject["heulsession_id"] = heulsession_id;
    String jsonString = JSON.stringify(dataObject);

    // WLAN
    if (WiFi.status() == WL_CONNECTED) {

        ////////////////////////////////////////////////////////////// start HTTP connecion and perform a POST query
        HTTPClient http;
        http.begin(serverURL);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonString);

        ////////////////////////////////////////////////////////////// process HTTP response
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.printf("HTTP Response code: %d\n", httpResponseCode);
            Serial.println("Response: " + response);

            // parse JSON response
            JSONVar myObject = JSON.parse(response);
            if (JSON.typeof(myObject) != "undefined") {
                if (myObject.hasOwnProperty("heulsession_id")) {
                    heulsession_id = getJsonInt(myObject["heulsession_id"]);
                    Serial.print("New heulsession_id stored: ");
                    Serial.println(heulsession_id);
                }
            } else {
                Serial.println("Parsing failed!");
            }
        } else {
            Serial.printf("Error on sending POST: %d\n", httpResponseCode);
        }
        http.end();
    }
}

// if the value is a string (eg. "19"), we need to convert it into an int
int getJsonInt(JSONVar idValue) {
    if (JSON.typeof(idValue) == "string") {
        String temp = (const char*)idValue;
        return temp.toInt();
    } 
    // parsing not necessary if the value is already an int
    else if (JSON.typeof(idValue) == "number") {
        return (int)idValue;
    }
    return 0;                                  // Fallback if neither string nor int
}