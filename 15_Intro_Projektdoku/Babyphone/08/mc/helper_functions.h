#include <HTTPClient.h>
#include <Arduino_JSON.h> 



///// smooting audio: if the audio level of x% from the rexent x seconds were above the threshold audio level, then is_heulsession is 1

#define BUFFER_SIZE_SMOOTH 25                    // check for audio volume every 100ms -> 25 Werte for 2.5s
int heul_history[BUFFER_SIZE_SMOOTH];
int history_index = 0;
unsigned long last_history_update = 0;
int is_heulsession = 0;   


// 70% LOGIC: is_heulsession = 1 if the audio volume > the threshold during 70% of the last x seconds --> bridging breaks
int isMostlyLoud(int current_noise_detected){
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
            return 1;
        } else {
            return 0;
        }
    }
}



// if the value  in the JSON array received from the database is a string (eg. "19"), we need to convert it into an int
int cast_int(JSONVar idValue) {
    if (JSON.typeof(idValue) == "string") {
        String temp = (const char*)idValue;
        return temp.toInt();
    } 
    
    else if (JSON.typeof(idValue) == "number") {      // parsing not necessary if the value is already an int
        return (int)idValue;
    }
    return 0;                                         // Fallback if neither string nor int
}


// called on setup() function, once at start: select the songs that should be played

int selected_tracks_ids[15];
int selected_tracks_titles[15];
int num_selected_tracks = 0;
int randomTrackIndex;

void updateSelectedTracks(){
    HTTPClient http;
    http.begin("https://heulradar.hausmaenner.ch/db/get_selected_tracks.php");

    int httpResponseCode = http.GET();                // Send the GET request
    if (httpResponseCode > 0) {
        String payload = http.getString();            // Get the response payload as a string
        Serial.println("Payload received:");
        Serial.println(payload);

        JSONVar myObject = JSON.parse(payload);

        if (JSON.typeof(myObject) == "undefined") {
            Serial.println("Parsing failed!");
        } else {
            for (int i = 0; i < 15 && i < myObject.length(); i++) {       // Access the "selected" field of each object
                selected_tracks_ids[i] = (int)myObject[i]["id"];
                selected_tracks_titles[i] = (int)myObject[i]["title"];
                num_selected_tracks++;
                
                Serial.print("Track ");
                Serial.print((int)myObject[i]["id"]);
                Serial.println((int)myObject[i]["title"]);
            }
        }
    } else {
        Serial.print("Error on sending GET: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}




// Pick a random track id (1-15) to play it
int getRandomTrackId() {
    randomTrackIndex = random(0, num_selected_tracks); 
    return selected_tracks_ids[randomTrackIndex];
}

int getRandomTrackName() {
    return selected_tracks_titles[randomTrackIndex];
}

