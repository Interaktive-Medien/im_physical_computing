/******************************************************************************************
 * mc.ino
 * Install library "Arduino_JSON" by Arduino
 * Sensordaten sammeln und per HTTP POST Request an Server schicken (-> load.php).
 * load.php schreibt die Werte dann in die Datenbank
 * Beachte: Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an.
 ******************************************************************************************/



#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> // by Arduino

unsigned long lastTime = 0;
unsigned long timerDelay = 15000; // alle 15s wird ein neuer Wert verschickt

const char* ssid     = "Igloo";
const char* pass     = "1glooVision";

// const char* serverURL = "https://192.168.0.98/05_mc2db/server2db.php"; // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
const char* serverURL = "https://fiessling.ch/iot1/hs24_sensor2website/load.php"; 

void setup() {
  Serial.begin(115200);
  

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
  
  if ((millis() - lastTime) > timerDelay) {   // alle 15 Sekunden...
    lastTime = millis();


    ////////////////////////////// sensor auslesen

    float wert = (float)random(0, 1000) / 10;  // 0 - 100
    Serial.println(wert);

    
    ////////////////////////////// JSON zusammenbauen

    JSONVar dataObject;
    dataObject["wert"] = wert;
    String jsonString = JSON.stringify(dataObject);

    ////////////////////////////// JSON string per HTTP POST request an den Server schicken (server2db.php)
    
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonString);
    http.end();
   
  }
}
