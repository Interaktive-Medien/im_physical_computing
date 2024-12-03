/******************************************************************************************
 * mc.ino
 * Button signal from ESP32 to TouchDesigner and LED data to ESP32 via OSC
 * LED leuchtet gelb, wenn der angeschlossene Hardware-Button am ESP32 gedrückt wird
 * LED leuchtet blau, wenn der ESP32 OSC-Nachricht mit Topic "from_td" und Payload 0 empfängt
 * Install library "OSC" by Adrian Freed
 * Install library "NeoPixel" by Adafruit
 * specify your Wifi ssid and pw, and IP address of your receiver PC
 ******************************************************************************************/


#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "tinkergarden";            // @todo: add your wifi name
const char* pass = "strenggeheim";            // @todo: add your wifi pw

WiFiUDP Udp;                               
const IPAddress remoteIp(192, 168, 0, 20);    // @todo: add receiver IP address
const unsigned int remotePort = 9000;          
const unsigned int localPort = 8000;        

// button
const int buttonPin = 7; 
int buttonState = 0;         
int prev_buttonState = 0;
const int led = RGB_BUILTIN;

// ws2812
const int stripPin = 6;      
const int num_leds = 10; 
Adafruit_NeoPixel strip(num_leds, stripPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  connectWiFi();
  connectUdp();
  setupStrip();

  pinMode(buttonPin, INPUT);   
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);   // LED off
}

void connectWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("WiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());
}

void connectUdp() {
  Udp.begin(localPort);
  Serial.println("Starting UDP - Local port: " + String(localPort));
}

void setupStrip(){
  strip.begin();       
  strip.show();            
  strip.setBrightness(255); 
}

void loop() {
  sendOSC(); 
  receiveOSC_StripData();   
}

void sendOSC() {
  ////////////////// get button value
  buttonState = digitalRead(buttonPin);
  if(buttonState == prev_buttonState) return;
  prev_buttonState = buttonState;

  ////////////////// feedback on serial port abd LED
  if (buttonState == 1) rgbLedWrite(led, 255, 255, 0);  // LED gelb
  else digitalWrite(led, 0);                            // LED aus
  Serial.println("/to_td: " + String(buttonState));

  ////////////////// send value via OSC
  OSCMessage msg("/to_td");        // define OSC key
  msg.add((int32_t)buttonState);   // define OSC value
  Udp.beginPacket(remoteIp, remotePort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void receiveOSC_StripData(){
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {
      for (int i = 0; i < bundle.size(); i++) {
        OSCMessage msg = bundle.getOSCMessage(i);

        // Serial.println("Empfangen: " + String(msg.getAddress()));   // testen ob überhaupt irgendetwas ankommt

        ////////////////// bestimmten OSC key empfangen und Aktion auslösen
        if (strcmp(msg.getAddress(), "/colors") == 0) {

          /////////// drive strip from OSC message
          for(int i=0; i < num_leds; i++) {
            strip.setPixelColor(i, msg.getInt(i*3), msg.getInt((i*3)+1), msg.getInt((i*3)+2));
          }
          strip.show(); // Update strip with new contents
          delay(15);   
        }
      }
    } else {
      // Serial.println("error: " + String(bundle.getError()));
    }
  }
}