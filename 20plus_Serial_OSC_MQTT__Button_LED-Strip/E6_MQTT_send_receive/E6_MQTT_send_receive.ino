/******************************************************************************************
 * Button Signal zu TouchDesigner aund zurück via MQTT
 * LED leuchtet gelb, wenn der angeschlossene Hardware-Button am ESP32 gedrückt wird
 * LED leuchtet blau, wenn der ESP32 OSC-Nachricht mit Topic "from_td" und Payload 1 empfängt
 * Installiere Library "MQTT" by Joel Gaehwiler
 * specify your Wifi ssid and pw, and IP address of your running MQTT message broker
 ******************************************************************************************/


///////////////////////////////////////////////// WiFi & MQTT

#include <WiFi.h>
#include <MQTT.h>

const char* ssid = "tinkergarden";             // @todo: add your wifi name
const char* pass = "strenggeheim";             // @todo: add your wifi pw
const char* broker = "192.168.0.102";          // @todo: add your broker ip address
const char* subscribe_topics[] = {"from_td"};
String publish_topic = "to_td";

WiFiClient wificlient;
MQTTClient mqttclient;

///////////////////////////////////////////////// Hardware-Pins

const int buttonPin = 7;                       // Pin für den Button
const int led = LED_BUILTIN;                   // Standard-LED-Pin auf den meisten ESP32-Boards

int buttonState = 0;
int prev_buttonState = 0;

// Funktionsprototypen

void connectWiFi();
void connectMQTT();
void messageReceived(String &topic, String &payload);
void sendMQTT();

void setup() {
  Serial.begin(115200);
  connectWiFi();
  connectMQTT();

  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);                        // LED aus
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


void loop() {
  mqttclient.loop();
  delay(10); // Stabilität
  if (!mqttclient.connected()) connectMQTT();
  sendMQTT();
}


///////////////////////////////////////////////// steuere LED bei entsprechender MQTT Nachricht

void messageReceived(String &topic, String &payload) {
  Serial.println("Incoming: " + topic + " - " + payload);
  if (topic == "from_td") {
    if (payload.toInt() == 1) {
      rgbLedWrite(led, 0, 0, 255);               // LED blau 
    } else {
      digitalWrite(led, 0); // LED aus
    }
  }
}


///////////////////////////////////////////////// sende MQTT Nachricht bei Knopfdruck

void sendMQTT() {

  ////////////////// get button value
  buttonState = digitalRead(buttonPin);
  if (buttonState == prev_buttonState) return;
  prev_buttonState = buttonState;

  ////////////////// feedback on serial port and LED
  if (buttonState == 1) rgbLedWrite(led, 255, 255, 0);  // LED gelb
  else digitalWrite(led, 0);                            // LED aus

  ////////////////// send value via MQTT
  Serial.println("Publishing: " + publish_topic + " - " + String(buttonState));
  mqttclient.publish(publish_topic, String(buttonState));
}
