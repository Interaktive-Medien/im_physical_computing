/******************************************************************************************
 * Button Signal zu TouchDesigner und zurück via MQTT (inkl. GRB-LED Status)
 *
 * LED Status-Indikatoren (GRB-Reihenfolge: Grün, Rot, Blau):
 * - Rot  (0, 255, 0):   Kein WLAN / Verbindungsaufbau
 * - Grün (255, 0, 0):   WLAN verbunden (sucht MQTT)
 * - Gelb (255, 255, 0): WLAN + MQTT verbunden
 * - Blau (0, 0, 255):   MQTT-Nachricht auf "erdgeschoss/wc/licht" mit Payload 1
 *empfangen
 * - Weiß (255, 255, 255): Hardware-Button an Pin 2 wird gedrückt
 ******************************************************************************************/

#include <MQTT.h>
#include <WiFi.h>

///////////////////////////////////////////////// WiFi & MQTT Konfiguration

const char *ssid = "tinkergarden";     // your WLAN ssid
const char *pass = "strenggeheim"; // your WLAN PW
bool isWlanConnected = 0;


const char *broker = "q7"; // falls kein mDNS verfügbar: nimm IP Addresse. Finde heraus am Raspberry pi via "ip addr" "192.168.0.51";
const int mqtt_port = 1883;

const char *subscribe_topics[] = {"erdgeschoss/wc/licht"};
String publish_topic = "erdgeschoss/wc/sensor";

WiFiClient wificlient;
MQTTClient mqttclient;

///////////////////////////////////////////////// Hardware-Pins

int led = LED_BUILTIN;
const int buttonPin = 7; // Pin für den Button

int buttonState = 0;
int prev_buttonState = 0;

// Funktionsprototypen
void connectWiFi();
bool is_wlan_connected();
void connectMQTT();
void messageReceived(String &topic, String &payload);
void sendMQTT();

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(led, OUTPUT);
  pinMode(buttonPin, INPUT_PULLDOWN);

  rgbLedWrite(led, 0, 10, 0); // GRB: rot
  Serial.println("Starte Verbindung...");

  connectWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    connectMQTT();
  }
}

void loop() {
  if (!is_wlan_connected())
    return;

  // Wenn WLAN steht, aber MQTT getrennt ist -> neu verbinden
  if (!mqttclient.connected()) {
    rgbLedWrite(led, 10, 0, 0); // GRB: grün (nur WLAN da, MQTT fehlt)
    connectMQTT();
  }

  mqttclient.loop();
  delay(10); // Stabilität
  sendMQTT();
}

///////////////////////////////////////////////// WLAN Funktionen (Dein
/// Boilerplate)

void connectWiFi() {
  Serial.printf("\nVerbinde mit WLAN %s", ssid);
  WiFi.begin(ssid, pass);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED &&
         attempts < 40) { // Max 40 Versuche (20 Sekunden)
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWiFi verbunden: SSID: %s, IP-Adresse: %s\n", ssid,
                  WiFi.localIP().toString().c_str());
    rgbLedWrite(led, 10, 0, 0); // GRB: grün
    isWlanConnected = 1;
  } else {
    Serial.println("\n❌ WiFi Verbindung fehlgeschlagen!");
    rgbLedWrite(led, 0, 10, 0); // GRB: rot
  }
}

bool is_wlan_connected() {
  if (WiFi.status() != WL_CONNECTED) {
    if (isWlanConnected == 1) { // War vorher verbunden?
      Serial.println("WiFi-Verbindung verloren, reconnect...");
      rgbLedWrite(led, 0, 10, 0); // GRB: Rot
      isWlanConnected = 0;
    }
    connectWiFi();
    return false; // Loop wird abgebrochen
  }
  return true; // WiFi ist da, Loop darf weiterlaufen
}

///////////////////////////////////////////////// MQTT Funktionen

void connectMQTT() {
  mqttclient.begin(broker, mqtt_port, wificlient);
  mqttclient.onMessage(messageReceived);

  Serial.print("Verbinde mit MQTT-Broker...");

  int attempts = 0;
  // Eigene Client-ID "esp32-c6-wc".
  // Der Mosquitto-Broker auf dem Raspberry Pi erwartet anonymous-Verbindungen
  // (ohne Benutzername/Passwort). Die Zugangsdaten "public", "public" führten
  // dazu, dass der Broker die Verbindung mit "Not authorised" (Code 5)
  // abgelehnt hat.
  while (!mqttclient.connect("esp32-c6-wc") && attempts < 5) {
    Serial.print(".");
    delay(1000);
    attempts++;
    if (WiFi.status() != WL_CONNECTED)
      return; // Abbruch falls WLAN wegfällt
  }

  if (mqttclient.connected()) {
    Serial.println("\nVerbunden mit MQTT-Broker!");

    int numTopics = sizeof(subscribe_topics) / sizeof(subscribe_topics[0]);
    for (int i = 0; i < numTopics; i++) {
      mqttclient.subscribe(subscribe_topics[i]);
    }

    rgbLedWrite(led, 10, 10,
                0); // GRB: gelb (Grün + Rot = Gelb -> WLAN & MQTT verbunden)
  } else {
    Serial.printf("\n❌ Fehler beim Verbinden mit MQTT-Broker! (Error: %d, "
                  "ReturnCode: %d)\n",
                  mqttclient.lastError(), mqttclient.returnCode());
  }
}

///////////////////////////////////////////////// steuere LED bei entsprechender
/// MQTT Nachricht

void messageReceived(String &topic, String &payload) {
  Serial.println("Incoming: " + topic + " - " + payload);
  if (topic == "erdgeschoss/wc/licht") {
    if (payload.toInt() == 1) {
      rgbLedWrite(led, 255, 255, 255); // GRB: blau
    } else {
      rgbLedWrite(led, 10, 10,
                  0); // GRB: zurück auf gelb (MQTT-Verbindungsstatus)
    }
  }
}

///////////////////////////////////////////////// sende MQTT Nachricht bei
/// Knopfdruck

void sendMQTT() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == prev_buttonState)
    return;
  prev_buttonState = buttonState;

  // Feedback auf LED beim Drücken (kurz Weiß, beim Loslassen wieder Gelb)
  if (buttonState == 1) {
    rgbLedWrite(led, 50, 50, 50); // GRB: weiß beim Drücken
  } else {
    rgbLedWrite(led, 10, 10, 0); // GRB: zurück auf gelb
  }

  // Wert via MQTT senden
  Serial.println("Publishing: " + publish_topic + " - " + String(buttonState));
  mqttclient.publish(publish_topic, String(buttonState));
}