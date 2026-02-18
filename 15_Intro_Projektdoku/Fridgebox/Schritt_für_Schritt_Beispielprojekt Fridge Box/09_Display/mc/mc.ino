/******************************************************************************************************
 * Fridge Box
 * Schritt 9: Integration OLED-Display

 ***************  Schritt 2: Reed-Schalter:
 * Reed-Sensor: eine Seite     <->  3.3V  
 * Reed-Sensor: andere Seite   <->  ESP32-C6: GPIO 10 (input)

 ***************  Schritt 5: Daten an die Datenbank schicken
 * - Passe den Pfad zur load.php in const char* serverURL auf deinen eigenen an
 * - Gib SSID und Passwort deines WLANs an
 * - Installiere Library "Arduino_JSON" by Arduino

 ***************  Schritt 6: Piepston nach 20s
 * 2-Bein-Piepser: längeres Bein     <->  ESP32-C6: GPIO 1 (output) 
 * 2-Bein-Piepser: kürzeres Bein     <->  GND  

 ***************  Schritt 7: Animation auf LED-Ring
 * WS2812B-Ring: Data In  <->  ESP32-C6: GPIO 2 (output) 
 * WS2812B-Ring: +5V/Vcc  <->  5V
 * WS2812B-Ring: GND      <->  GND
 * - Installiere Library "Adafruit Neopixel" by Adafruit

 ***************  Schritt 9: Integration OLED-Display
 * SSD1306: VDD  <->  3.3V
 * SSD1306: GND  <->  GND
 * SSD1306: SDA  <->  ESP32-C6: GPIO6
 * SSD1306: SCK  <->  ESP32-C6: GPIO7
 * - Installiere Library "Adafruit SSD1306" by Adafruit
 
 ******************************************************************************************************/

/***************************
* WLAN und HTTP
***************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h> 

/***************************
* LED-Ring
***************************/

#include <Adafruit_NeoPixel.h>
#include <math.h> // für Gamma-Korrektur bei Lichtanim -> pow()

/***************************
* OLED-Display
***************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


int ledPin = BUILTIN_LED;                         


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

const char* ssid      = "tinkergarden";                           // WLAN
const char* pass      = "strenggeheim";                           // WLAN
const char* serverURL = "https://fiessling.ch/physco/load.php";   // Server-Adresse: hier kann http oder https stehen, aber nicht ohne
  
/***************************
* Piepser
***************************/

int piepserPin = 1; 

int toleranz_zeit = 10000;                                // Zeit in ms nach Öffnung des Kühlschranks, bevor das Piepsen los geht
int beep_frequenz = 1000;                                 // Zeit in ms, wie lange piepston jeweils an oder aus ist
unsigned long prev_beep_anaus_changed_timestamp = 0;      // Zeit in ms, wann Piepston zuletzt an- oder ausgeschalten wurde
bool is_currently_noising = false;                        // wenn gerade piepsen soll: hier wird gespeichert, ob gerade Ton an oder aus ist

/***************************
* LED-Ring
***************************/

#define LEDRING_PIN 2
#define NUM_LEDRING_PIXELS 12

Adafruit_NeoPixel ledring = Adafruit_NeoPixel(NUM_LEDRING_PIXELS, LEDRING_PIN, NEO_GRB + NEO_KHZ800);

// Variablen für non-blocking Timing
unsigned long ledring_previous_update_timestamp = 0;

// Variablen für Lauflicht
int ledring_center = 0;
const long ledring_lauflicht_interval = 80; // Lauflicht Schrittzeit

// Variablen für Puls-Animation
int ledring_pulse_helligkeit = 0;           // 0-255 linear
int ledring_pulse_richtung = 1;             // 1 = heller werden, -1 = dunkler werden
const long ledring_pulse_interval = 70;     // Pulsier Schrittzeit

// Aktuelle Modus-Variable
int ledring_modus = 0;                      // 0 = aus, 1 = Pulsieren, 2 = rotes Lauflicht


/***************************
* OLED-Display
***************************/

#define I2C_SDA 6
#define I2C_SCL 7   
#define I2C_ADDRESS   0x3C                   // initialisiere mit Standard I2C addr 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);


                                                            
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);                    
  digitalWrite(ledPin, 0);

  /***************************
  * sensor
  ***************************/

  pinMode(sensorPin, INPUT_PULLDOWN);

  /***************************
  * WLAN
  ***************************/

  connectWiFi();

  /***************************
  * Piepser
  ***************************/

  pinMode(piepserPin, OUTPUT);
  digitalWrite(piepserPin, 0);
  beep(false);

  /***************************
  * LED-Ring
  ***************************/

  ledring.begin();
  ledring.setBrightness(100);            // Grundhelligkeit (0 - 255)

  /***************************
  * OLED-Display
  ***************************/

  Wire.begin(I2C_SDA, I2C_SCL);              // I2C mit eigenen Pins initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20, 20);
  display.println("Starte Fridge Box");
  display.display();

}

void loop() {

  /***************************
  * LED-RING
  ***************************/


  if (ledring_modus == 0) {
    ledring_aus();                        // LED-Ring ausschalten
  } else if (ledring_modus == 1) {
    ledring_pulsiere_blau();
  } else if (ledring_modus == 2) {
    ledring_rotiere_rotes_lauflicht();
  }




  /***************************
  * Sensorwert auslesen
  ***************************/

  sensor_state = digitalRead(sensorPin);

  if ((millis() - start_open_timestamp) > toleranz_zeit && sensor_state == 0) {      // neu: akustisches Signal nur wenn Toleranzzeit überschritten ist und wenn kein Magnet erkannt wird
    beep(true);
    ledring_modus = 2;     // LED-Ring Lauflicht
  }

  if(sensor_state == prev_sensor_state){
    return;
  }
  prev_sensor_state = sensor_state;

  if(sensor_state == 1){               // Kühlschrank schliesst
    Serial.println("Kühlschrank schliesst");
    digitalWrite(ledPin, 0);
    digitalWrite(piepserPin, 0);
    ledring_modus = 0;                 // LED-Ring aus
    display_aus();
    
  } else{                              // Kühlschrank öffnet
    Serial.println("Kühlschrank öffnet");
    digitalWrite(ledPin, 1);
    display_show_info();

    /***************************
    * Piepser
    ***************************/
    
    digitalWrite(piepserPin, 0);       // muss noch explizit ausgeschalten werden, sonst endet das Piepsen im Dauerton
    is_currently_noising = false;      // Piepser ausschalten!
    start_open_timestamp = millis();
    beep(false);                       // geht erst nach Toleranzzeit los

    /***************************
    * JSON zusammenbauen
    ***************************/

    int wert = 0;
    String jsonString = constructJson(wert);

    /***************************
    * JSON string per HTTP POST request an den Server schicken (load.php)
    ***************************/

    sendMsgToServer(jsonString);


    /***************************
    * LED-RING
    ***************************/

    ledring_modus = 1;     // LED-Ring Pulse
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

/*******************************
 * Piepser
********************************/

void beep(bool beepState){
  if(beepState == false){
    digitalWrite(piepserPin, 0);  // mach Piepser stumm
  }
  else {   // if(beepState == true)
    if ((millis() - prev_beep_anaus_changed_timestamp) > beep_frequenz) {      // neu: akustisches Signal nur wenn Toleranzzeit überschritten ist und wenn kein Magnet erkannt wird
      prev_beep_anaus_changed_timestamp = millis();
      is_currently_noising = !is_currently_noising;
      digitalWrite(piepserPin, is_currently_noising);
    }  
  }
}


/*******************************
 * LED-Ring
********************************/

// Funktion für den ausgeschalteten LED-Ring (komplett schwarz)
void ledring_aus() {
  for (int i = 0; i < NUM_LEDRING_PIXELS; i++) {
    ledring.setPixelColor(i, ledring.Color(0, 0, 0)); // Setzt alle LEDs auf Schwarz
  }
  ledring.show(); // Anwenden
}


void ledring_rotiere_rotes_lauflicht() {
  unsigned long currentMillis = millis();

  if (currentMillis - ledring_previous_update_timestamp >= ledring_lauflicht_interval) {
    ledring_previous_update_timestamp = currentMillis;

    ledring.clear();

    // Sanfter Verlauf über 7 LEDs: -3, -2, -1, 0, +1, +2, +3
    for (int offset = -3; offset <= 3; offset++) {
      int pixel = (ledring_center + offset + NUM_LEDRING_PIXELS) % NUM_LEDRING_PIXELS; // wrap-around Ring

      uint8_t brightnessFactor;
      switch (offset) {
        case 0:  brightnessFactor = 255; break; // Zentrum voll hell
        case -1: case 1: brightnessFactor = 110; break; // erste Nachbarn mittelhell
        case -2: case 2: brightnessFactor = 25; break; // äußere Nachbarn schwach
        case -3: case 3: brightnessFactor = 7; break; // äußerste schwach
      }
      
      ledring.setPixelColor(pixel, ledring.Color(brightnessFactor, 0, 0)); // Rot
    }

    ledring.show();

    ledring_center = (ledring_center + 1) % NUM_LEDRING_PIXELS; // einen Schritt weiter
  }
}


void ledring_pulsiere_blau() {
  unsigned long currentMillis = millis();

  if (currentMillis - ledring_previous_update_timestamp >= ledring_pulse_interval) {
    ledring_previous_update_timestamp = currentMillis;

    // Lineare Helligkeit anpassen
    ledring_pulse_helligkeit += ledring_pulse_richtung * 5;

    // Grenzen prüfen und Richtung umkehren
    if (ledring_pulse_helligkeit >= 255) {
      ledring_pulse_helligkeit = 255;
      ledring_pulse_richtung = -1;
    } else if (ledring_pulse_helligkeit <= 0) {
      ledring_pulse_helligkeit = 0;
      ledring_pulse_richtung = 1;
    }

    // Gammakorrektur anwenden (Gamma ≈ 2.2)
    float normalized = ledring_pulse_helligkeit / 255.0;
    float gammaCorrected = pow(normalized, 2.2); // Wertebereich 0.0 - 1.0

    // Minimalwerte für dunkles Blau definieren
    const float MIN_B = 0.06;  // 6% Blau
    const float MIN_G = 0.0;   // 0% Grün

    // Farbwerte skalieren
    float g_float = MIN_G + (0.7 - MIN_G) * gammaCorrected;
    float b_float = MIN_B + (1.0 - MIN_B) * gammaCorrected;

    uint8_t r = 0;
    uint8_t g = (uint8_t)(g_float * 255.0);
    uint8_t b = (uint8_t)(b_float * 255.0);

    // LEDs setzen
    for (int i = 0; i < NUM_LEDRING_PIXELS; i++) {
      ledring.setPixelColor(i, ledring.Color(r, g, b));
    }

    ledring.show();
  }
}



void display_show_info(){
  display.println("Close the fridge");
  display.display();
}

void display_aus(){
  display.clearDisplay();
  display.display();
}
