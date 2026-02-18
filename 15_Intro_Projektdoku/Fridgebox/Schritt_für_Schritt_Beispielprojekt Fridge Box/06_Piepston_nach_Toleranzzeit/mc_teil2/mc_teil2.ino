/******************************************************************************************************
 * Fridge Box
 * Schritt 6: Piepston nach 20s

 ***************  Schritt 2: Reed-Schalter:
 * Reed-Sensor: eine Seite     <->  3.3V  
 * Reed-Sensor: andere Seite   <->  ESP32-C6: GPIO 10 (input)

 ***************  Schritt 5: Daten an die Datenbank schicken
 * - Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an
 * - Gib SSID und Passwort deines WLANs an
 * - Installiere Library "Arduino_JSON" by Arduino

 ***************  Schritt 6: Piepston nach 20s
 * 2-Bein-Piepser: längeres Bein     <->  ESP32-C6: GPIO 1 (output) 
 * 2-Bein-Piepser: kürzeres Bein     <->  GND (output) 


 " Hier kein Dauerton, sondern Piepsen"
 * json packing und http-versand ausgelagert in Funktionen, damit loop() lesbarer ist
 ******************************************************************************************************/



#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 


int ledPin = BUILTIN_LED;                            // neu: akustisches Signal


/***************************
* sensor
***************************/

int sensorPin = 10;
int sensor_state = 0;
int prev_sensor_state = 0;
unsigned long start_open_timestamp = 0;

/***************************
* WLAN und Server
***************************/

const char* ssid     = "tinkergarden";                           // WLAN
const char* pass     = "strenggeheim";                           // WLAN
const char* serverURL = "https://fiessling.ch/physco/load.php";  // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
  
/***************************
* Piepser
***************************/

int piepserPin = 1; 

int toleranz_zeit = 10000;                                // Zeit in ms nach Öffnung des Kühlschranks, bevor das Piepsen los geht
int beep_frequenz = 1000;                                 // Zeit in ms, wie lange piepston jeweils an oder aus ist
unsigned long prev_beep_anaus_changed_timestamp = 0;      // Zeit in ms, wann Piepston zuletzt an- oder ausgeschalten wurde
bool is_currently_noising = false;                        // wenn gerade piepsen soll: hier wird gespeichert, ob gerade Ton an oder aus ist



                                                            
void setup() {
  Serial.begin(115200);
  
  pinMode(sensorPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  pinMode(piepserPin, OUTPUT);                   // neu: akustisches Signal
  digitalWrite(ledPin, 0);
  digitalWrite(piepserPin, 0);                   // neu: akustisches Signal

  /***************************
  * WLAN
  ***************************/

  connectWiFi();
}

void loop() {

  /***************************
  * Sensorwert auslesen
  ***************************/

  sensor_state = digitalRead(sensorPin);

  if ((millis() - start_open_timestamp) > toleranz_zeit && sensor_state == 0) {      // neu: akustisches Signal nur wenn Toleranzzeit überschritten ist und wenn kein Magnet erkannt wird
    beep(true);
  }


  if(sensor_state == prev_sensor_state){
    return;
  }
  prev_sensor_state = sensor_state;

  if(sensor_state == 1){  // Kühlschrank schliesst
    Serial.println("Kühlschrank schliesst");
    digitalWrite(ledPin, 0);
  }

  else{                   // Kühlschrank öffnet
    Serial.println("Kühlschrank öffnet");
    digitalWrite(ledPin, 1);
    beep(false);                       // geht erst nach Toleranzzeit los
    is_currently_noising = false;     // ← Zustand sicher auf AUS setzen!
    start_open_timestamp = millis();

    /***************************
    * JSON zusammenbauen
    ***************************/

    int wert = 0;
    String jsonString = constructJson(wert);


    /***************************
    * JSON string per HTTP POST request an den Server schicken (load.php)
    ***************************/

    sendMsgToServer(jsonString)
  }
}

/***************************
* WLAN
***************************/

void connectWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi...");
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\nWiFi connection failed, continuing without WiFi.");
  }
}


/*******************************
 * JSON zusammenbauen
********************************/

String constructJson(int wert){
  JSONVar dataObject;
  dataObject["wert"] = wert;
  // dataObject["wert2"] = wert2;
  String jsonString = JSON.stringify(dataObject);
  // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77}";  // stattdessen könnte man den JSON string auch so zusammenbauen
  return jsonString;
}



void sendMsgToServer(String jsonString){
  if (WiFi.status() == WL_CONNECTED) {                        // Überprüfen, ob Wi-Fi verbunden ist
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
      } else {
        Serial.printf("Error on sending POST: %d\n", httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
}


void beep(bool beepState){
  if(beepState == false){
    digitalWrite(piepserPin, 0);  // mach Piepser stumm
  }
  else {   // beepState == true
    Serial.println("beginne Piepston nach Öffnung des Kühlschranks + Toleranzzeit");
    if ((millis() - prev_beep_anaus_changed_timestamp) > beep_frequenz) {      // neu: akustisches Signal nur wenn Toleranzzeit überschritten ist und wenn kein Magnet erkannt wird
      prev_beep_anaus_changed_timestamp = millis();
      is_currently_noising = !is_currently_noising;
      digitalWrite(piepserPin, is_currently_noising);
    }  
  }
}
