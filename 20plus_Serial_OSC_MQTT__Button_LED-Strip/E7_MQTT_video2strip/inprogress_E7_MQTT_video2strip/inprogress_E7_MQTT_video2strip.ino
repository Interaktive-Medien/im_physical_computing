// button to MQTT and MQTT to LED
// load library "MQTT" by Joel Gaehwiler
// specify your Wifi ssid and pw, and IP address of your broker

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <Adafruit_NeoPixel.h>

const char* ssid[] = {"dreammakers", "Rosenweg Netz", "2hot4u"};   // @todo: add your wifi name
const char* pass[] = {"dreammakers", "9boeschnet",    "88288828"};  // @todo: add your wifi pw
const char* broker[] = {"192.168.0.80", "192.168.1.51"};  // @todo: add your broker ip address
const char* subscribe_topics[] = {"from_td", "colors"};
String publish_topic = "to_td";

WiFiClient wificlient;
MQTTClient mqttclient;

// button
const int buttonPin = 13;
int buttonState = 0;         
int prev_buttonState = 0;
// onboard leds
const int led1Pin =  2;    // standard BUILTIN_LED == 2
const int led2Pin =  16;   // 2nd builtin LED == 16
// ws2812
const int stripPin =  15;      // == D8 on NodeMCU -> this is where the running light strip are attached to NodeMCU
const int num_leds = 30; 
Adafruit_NeoPixel strip(num_leds, stripPin, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  connectWiFi();
  connectMQTT();
  setupStrip();

  pinMode(buttonPin, INPUT);   
  pinMode(led1Pin, OUTPUT);
  digitalWrite(led1Pin, 1);   // LED off
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led2Pin, 1);   // LED off
}

void connectWiFi() {
  // ESP8266 gets a dynamic ip address. Specify a static IP address with following line
  // WiFi.config(IPAddress(192,168,0,123),IPAddress(192,168,0,1), IPAddress(255,255,255,0)); 
  int num_networks = sizeof(ssid) / sizeof(ssid[0]);
  for (int i = 0; i < num_networks; i++) {
    Serial.print("Connecting to WiFi ");
    Serial.println(ssid[i]);
    WiFi.begin(ssid[i], pass[i]);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 8) {
      delay(1000);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected");
      return;
    }
  }
  Serial.println("\nUnable to connect to any WiFi network");
}

void connectMQTT() {
  int numBrokers = sizeof(broker) / sizeof(broker[0]);
  for (int i = 0; i < numBrokers; i++) {
    Serial.print("Connecting to broker ");
    Serial.print(broker[i]); 
    Serial.println("...");
    mqttclient.begin(broker[i], wificlient);    
    if (mqttclient.connect("arduino", "public", "public")) { // client id, usr name, pw
      Serial.println("Verbunden mit MQTT-Broker");
      break;
    } else Serial.println("Fehler beim Verbinden mit MQTT-Broker");
  }

  int numTopics = sizeof(subscribe_topics) / sizeof(subscribe_topics[0]);
  for (int i = 0; i < numTopics; i++) {
    mqttclient.subscribe(subscribe_topics[i]); 
  }    
  mqttclient.onMessage(messageReceived);

}


void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == "from_td"){
    if (payload.toInt() == 1) digitalWrite(led2Pin, 0); 
    else digitalWrite(led2Pin, 1); 
  }
  else if (topic == "colors"){
    Serial.println("colors");
    Serial.println("payload: " + payload);
    const int maxSize = 45; // Größe des Arrays entsprechend der Anzahl der Zahlen im String



    int intArray[maxSize];
    int arrayIndex = 0;
    String tempStr = payload;
    tempStr = "22,0,0,0,22,0,0,0,22,22,0,0,0,22,0,0,0,22,22,0,0,0,22,0,0,0,22,22,0,0,0,22,0,0,0,22,22,0,0,0,22,0,0,0,22";
    while (tempStr.length() > 0) {
      int commaIndex = tempStr.indexOf(',');
      if (commaIndex != -1) {
        intArray[arrayIndex] = tempStr.substring(0, commaIndex).toInt();
        tempStr = tempStr.substring(commaIndex + 1);
      } else {
        intArray[arrayIndex] = tempStr.toInt();
        tempStr = ""; // Der String ist leer
      }
      arrayIndex++;
    }
    //if (payload.toInt() == 1) digitalWrite(led2Pin, 0); 
    //else digitalWrite(led2Pin, 1); 

    
    // drive strip from OSC message
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, intArray[i*3], intArray[i*3+1], intArray[i*3+2]);
      Serial.println("R: " + String(intArray[i*3]) + " G: " + String(intArray[i*3+1]) + " B: " + String(intArray[i*3+2]));
    }
    strip.show(); // Update strip with new contents
    delay(15);    // Pause for a moment
  }
}

void setupStrip(){
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            
  strip.setBrightness(255); 
}

void loop() {
  mqttclient.loop();
  delay(10);     // <- fixes some issues with WiFi stability
  if (!mqttclient.connected()) connectMQTT();
  sendToTD();
}

// send button data to TouchDesigner
void sendToTD(){
  buttonState = digitalRead(buttonPin);
  if(buttonState == prev_buttonState) return;
  prev_buttonState = buttonState;


  // check if the pushbutton is pressed. If it is, the buttonState is 1:
  Serial.println("Outgoing: " + publish_topic + " - " + String(buttonState));
  mqttclient.publish(publish_topic, String(buttonState));
  digitalWrite(led1Pin, (buttonState + 1) % 2); // 0 == on, 1 == off
}
