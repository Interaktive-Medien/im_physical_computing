/******************************************************************************************
 * Video2Strip_MQTT.ino
 * Empfange Lichtdaten per MQTT von z. B. TouchDesigner
 * LED-Ring zeigt Video, das in TouchDesigner für die LEDs portioniert wurde
 * LED leuchtet gelb, wenn der angeschlossene Hardware-Button am ESP32 gedrückt wird (MQTT msg to TouchDesigner)
 * LED leuchtet blau, wenn der ESP32 OSC-Nachricht mit Topic "from_td" und Payload 1 empfängt
 * Installiere Libraries "MQTT" by Joel Gaehwiler und "Adafruit NeoPixel" by Adafruit
 * specify your Wifi ssid and pw, and IP address of your running MQTT message broker
 ******************************************************************************************/


#include <Adafruit_NeoPixel.h>

///////////////////////////////////////////////// WiFi & MQTT

#include <WiFi.h>
#include <MQTT.h>

const char* ssid = "tinkergarden";             // @todo: add your wifi name
const char* pass = "strenggeheim";             // @todo: add your wifi pw
const char* broker = "192.168.0.102";          // @todo: add your broker ip address
const char* subscribe_topics[] = {"from_td", "colors"};
String publish_topic = "to_td";

WiFiClient wificlient;
MQTTClient mqttclient;


///////////////////////////////////////////////// Button

const int buttonPin = 7; 
int buttonState = 0;         
int prev_buttonState = 0;
const int led = RGB_BUILTIN;

////////////////////////////////////////////////// LED-Strip

const int stripPin = 2;      
const int num_leds = 12; 
Adafruit_NeoPixel strip(num_leds, stripPin, NEO_GRB + NEO_KHZ800);

void connectWiFi();
void connectMQTT();
void messageReceived(String &topic, String &payload);
void sendToTD();




// Funktionsprototypen

void setup() {
  Serial.begin(115200);
  connectWiFi();
  connectMQTT();
  setupStrip();

  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0); 
}


///////////////////////////////////////////////// WiFi & MQTT

void connectWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("WiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());
}

void connectMQTT() {
  mqttclient.begin(broker, wificlient);

  while (!mqttclient.connect("arduino", "public", "public")) {
    Serial.println("Fehler beim Verbinden mit MQTT-Broker. Neuer Versuch in 2 Sekunden...");
    delay(2000);
  }

  Serial.println("Verbunden mit MQTT-Broker");

  int numTopics = sizeof(subscribe_topics) / sizeof(subscribe_topics[0]);
  for (int i = 0; i < numTopics; i++) {
    mqttclient.subscribe(subscribe_topics[i]);
  }
  mqttclient.onMessage(messageReceived);
}

////////////////////////////////////////////////// LED-Strip

void setupStrip(){
  strip.begin();       
  strip.show();            
  strip.setBrightness(255); 
}


void loop() {
  mqttclient.loop();
  delay(10);
  if (!mqttclient.connected()) connectMQTT();
  sendMQTT();
}


///////////////////////////////////////////////// steuere LED strip bei entsprechender MQTT Nachricht (callback Funktion: see mqttclient.onMessage(messageReceived);)


void messageReceived(String &topic, String &payload) {
  // Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "colors") { 
    int values[num_leds * 3];  // RGB-Werte als Integer speichern
    
    // wandle CSV String in ein Int-Array um
    int index = 0;
    char *token = strtok((char*)payload.c_str(), ","); // Zerlege String an Kommas

    while (token != nullptr && index < num_leds * 3) {
      values[index++] = atoi(token);  // String in Integer umwandeln
      token = strtok(nullptr, ",");
    }

    // schreibe Werte aus dem Int-Array aan die WS2812B LEDs
    for (int i = 0; i < num_leds; i++) {
      strip.setPixelColor(i, strip.Color(values[i * 3], values[i * 3 + 1], values[i * 3 + 2]));
    }

    strip.show(); // Update LEDs
    delay(15);
  }
}




///////////////////////////////////////////////// sende MQTT Nachricht bei Knopfdruck

void sendMQTT(){

  ////////////////// get button value
  buttonState = digitalRead(buttonPin);
  if(buttonState == prev_buttonState) return;
  prev_buttonState = buttonState;

  ////////////////// feedback on serial port and LED
  if (buttonState == 1) rgbLedWrite(led, 255, 255, 0);  // LED gelb
  else digitalWrite(led, 0);                            // LED aus

  ////////////////// send value via MQTT
  
  Serial.println("Publishing: " + publish_topic + " - " + String(buttonState));
  mqttclient.publish(publish_topic, String(buttonState));
}
