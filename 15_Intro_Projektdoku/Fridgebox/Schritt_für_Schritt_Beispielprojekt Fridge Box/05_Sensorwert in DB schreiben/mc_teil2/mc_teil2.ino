/******************************************************************************************************
 * Fridge Box
 * Schritt 5: ESensordaten des Reed-Schalters an load.php schicken.
 * load.php schreibt die Werte dann in die Datenbank

 ***************  Schritt 2: Reed-Schalter:
 * Reed-Sensor: eine Seite     <->  3.3V  
 * Reed-Sensor: andere Seite   <->  ESP32-C6: GPIO 10 (input)

 ***************  Schritt 5: Daten an die Datenbank schicken
 * - Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an
 * - Gib SSID und Passwort deines WLANs an
 * - Installiere Library "Arduino_JSON" by Arduino

 * nun ist der Verbindungsprozess zum WLAN in der setup()-Funktion ausgelagert in eine separate Funktion. 
 * Wenn keinen Verbindung nach längerer Zeit, geht es nun trotzdem offline weiter.
 ******************************************************************************************************/



#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 

int sensorPin = 10;
int ledPin = BUILTIN_LED;

int sensorState = 0;
int prev_sensorState = 0;

const char* ssid     = "tinkergarden";                           // WLAN
const char* pass     = "strenggeheim";                           // WLAN

const char* serverURL = "https://fiessling.ch/physco/load.php";  // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
                                                              
void setup() {
  Serial.begin(115200);
  
  pinMode(sensorPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);

  /***************************
  * WLAN
  ***************************/

  connectWiFi();
}

void loop() {

  ////////////////////////////////////////////////////////////// Sensorwert auslesen

  sensorState = digitalRead(sensorPin);
  if(sensorState == prev_sensorState){
    return;
  }
  prev_sensorState = sensorState;

  if(sensorState == 1){  // Kühlschrank zu
    digitalWrite(ledPin, 0);
  }

  else{      // Kühlschrank offen
    digitalWrite(ledPin, 1);





    ////////////////////////////////////////////////////////////// JSON zusammenbauen
    int wert = undefined;
    JSONVar dataObject;
    dataObject["wert"] = wert;
    // dataObject["wert2"] = wert2;
    String jsonString = JSON.stringify(dataObject);
    // String jsonString = "{\"sensor\":\"fiessling\", \"wert\":77}";  // stattdessen könnte man den JSON string auch so zusammenbauen

  
     ////////////////////////////////////////////////////////////// JSON string per HTTP POST request an den Server schicken (server2db.php)

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
