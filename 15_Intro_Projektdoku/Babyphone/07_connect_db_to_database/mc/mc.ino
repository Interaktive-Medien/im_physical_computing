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
* Installiere Library "Arduino_JSON" by Arduino
 *
 * You probably need to shift the value of AUDIOVOLUME_OFFSET in get_audiovolume.h(adjust the decible threshold from when it should trigger). 
 * define AUDIOVOLUME_THRESHOLD, from which loudness on the triggerstate should me 1
 * For testing use a smartphone decible tester like https://apps.apple.com/ch/app/dezibel-x-dba-l%C3%A4rm-messger%C3%A4t/id448155923?l=de-DE
 ******************************************************************************************************/

int ledPin = BUILTIN_LED;

// WLAN + Server
#include <WiFi.h>
#include <HTTPClient.h>
// #include "connectWiFi_hochschule.h"   // activate this line when aonnecting with edu network (eg. eduroam)
#include "connectWiFi_zuhause.h"         // activate this line when connecting with home network
const char* serverURL = "https://heulradar.hausmaenner.ch/db/load.php";
#include <Arduino_JSON.h> 
int heulsession_id = 0;                  // entry id from database table will be stored here


// audio trigger
#include "get_audiovolume.h"
#include "audioplayer.h"
#define AUDIOVOLUME_THRESHOLD 60
#define TIME_UNTIL_PLAY 2000
int triggerState = 0;         
int prev_triggerState = 0;
unsigned long audiotrigger_startTime = 0;
bool audio_played = false;





void setup() {
  Serial.begin(115200);
  delay(3000);
  setup_audiovolume_tester();
  initAudioPlayer();             // function is in audioplayer.h
  connectWiFi();                 // connectWiFi() ist in connectWiFi_hochschule.h UND connectWiFi_zuhause.h. Zutreffendes oben aktivieren


  // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);

  // WLAN
  Serial.println("Start connection...");
  connectWiFi();                 // connectWiFi() ist in connectWiFi_hochschule.h UND connectWiFi_zuhause.h. Zutreffendes oben aktivieren
}

void loop(){

    // audiotrigger
    float audiovolume = get_audiovolume();
    // Serial.println("Min:20,Max:80,Audio_Volume:");
    // Serial.println(audiovolume); 
    
    if(audiovolume > AUDIOVOLUME_THRESHOLD){
        triggerState = 1;
    }
    else{
        triggerState = 0;
    }



    // case 1: audio trigger just detected just pressed
    if (triggerState == 1 && prev_triggerState == 0) {
        audiotrigger_startTime = millis();         // remember start time
        audio_played = false;              // Reset für diesen Tastendruck
        digitalWrite(ledPin, 1);           // turn LED on for feedback
    }

    // case 2: audio trigger has been detected already before and is still active -> play audio if mic detects loud noise long enough without interrupt
    if (triggerState == 1 && !audio_played) {
        if (millis() - audiotrigger_startTime >= TIME_UNTIL_PLAY) { 
            Serial.println("fire");
            playTrack(1);
            audio_played = true;  
            save_into_db(triggerState);           
        }
    }

    // case 3: audio trigger is not being detected anymore.
    if (triggerState == 0 && prev_triggerState == 1) {
        digitalWrite(ledPin, 0);             // LED off
        save_into_db(triggerState);  
    }

    prev_triggerState = triggerState;

    
    // WLAN
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi-Verbindung verloren, reconnect...");
        connectWiFi();
    }
    
}

void save_into_db(int triggerState){

    ////////////////////////////////////////////////////////////// JSON zusammenbauen

    JSONVar dataObject;
    dataObject["is_heulsession"] = triggerState;
    dataObject["heulsession_id"] = heulsession_id;
    String jsonString = JSON.stringify(dataObject);
    // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77}";  // stattdessen könnte man den JSON string auch so zusammenbauen


    // WLAN
    if (WiFi.status() == WL_CONNECTED) {

        // HTTP Verbindung starten und POST-Anfrage senden
        HTTPClient http;
        http.begin(serverURL);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonString);

        // Prüfen der Antwort
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.printf("HTTP Response code: %d\n", httpResponseCode);
            Serial.println("Response: " + response);

            // JSON-Antwort parsen
            JSONVar myObject = JSON.parse(response);




            // Überprüfen, ob das Parsen erfolgreich war und der Key existiert
            if (JSON.typeof(myObject) != "undefined") {
                if (myObject.hasOwnProperty("heulsession_id")) {
                    // Wichtig: Den Wert in einen Integer casten
                    Serial.print("received from Object: ");
                    Serial.println(myObject["heulsession_id"]);

                    heulsession_id = getJsonInt(myObject["heulsession_id"]);


                    Serial.print("Gespeicherte Session ID: ");
                    Serial.println(heulsession_id);
                }
            } else {
                Serial.println("Parsing fehlgeschlagen!");
            }








        } else {
            Serial.printf("Error on sending POST: %d\n", httpResponseCode);
        }
        http.end();
    }
}

int getJsonInt(JSONVar idValue) {
    // Prüfen, ob der Wert ein String ist (z.B. "19")
    if (JSON.typeof(idValue) == "string") {
        // Bei JSONVar extrahiert man den Text mit (const char*)
        String temp = (const char*)idValue;
        return temp.toInt();
    } 
    // Prüfen, ob der Wert direkt eine Zahl ist (z.B. 19 oder 0)
    else if (JSON.typeof(idValue) == "number") {
        return (int)idValue;
    }
    
    return 0; // Fallback, falls weder String noch Zahl
}