/******************************************************************************************
 * Kap. 13: Sende Sensordaten an Server
 * mc.ino
 * Installiere Library "Arduino_JSON" by Arduino
 * Sensordaten sammeln und per HTTP POST Request an Server schicken (-> an load.php).
 * load.php schreibt die Werte dann in die Datenbank
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 * Gib SSID und Passwort deines WLANs an.
 * Ersetze den Block "Sensor auslesen" durch tatsächliche Sensorwerte.
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/tree/main/13_MC2DB/mc/mc.ino
 ******************************************************************************************/



#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 

int sensorPin = 7;
int ledPin = BUILTIN_LED;

int sensorState = 0;
int prev_sensorState = 0;

const char* ssid     = "Difix";                          // WLAN
const char* pass     = "Difix88288828";                      // WLAN

const char* serverURL = "https://fiessling.ch/physco/load.php";  // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
                                                               // Beispiel: https://fiessling.ch/im4/18_mc2db/load.php
void setup() {
  Serial.begin(115200);
  
  pinMode(sensorPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);


  ////////////////////////////////////////////////////////////// Verbindung mit Wi-Fi herstellen

  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());
}

void loop() {
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
