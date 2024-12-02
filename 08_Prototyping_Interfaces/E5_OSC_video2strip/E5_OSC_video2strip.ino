// Button signal from ESP32 to TouchDesigner and LED data to ESP32 via OSC
// load library "OSC" by Adrian Freed
// load library "NeoPixel" by Adafruit
// specify your Wifi ssid and pw, and IP address of your TouchDesigner PC

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Adafruit_NeoPixel.h>

const char* ssid[] = {"tinkergarden", "Igloo"};   // @todo: add your wifi name
const char* pass[] = {"strenggeheim", "1glooVision"};  // @todo: add your wifi pw

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress remoteIp(192,168,0,87);        // @todo: add receiver IP address
const unsigned int remotePort = 10000;          // This is the default port of the oscinChop in TouchDesigner
const unsigned int localPort = 10000;        // local port to listen for OSC packets (actually not used for sending)

// button
const int buttonPin = 7; // == D7 on NodeMCU
int buttonState = 0;         
int prev_buttonState = 0;
const int led =  RGB_BUILTIN;

// ws2812
const int stripPin = 6;      // == D8 on NodeMCU -> this is where the running light strip are attached to NodeMCU
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
  // ESP8266 gets a dynamic ip address. Specify a static IP address with following line
  // WiFi.config(IPAddress(192,168,0,123),IPAddress(192,168,0,1), IPAddress(255,255,255,0)); 
  int num_networks = sizeof(ssid) / sizeof(ssid[0]);
  for (int i = 0; i < num_networks; i++) {
    Serial.print("Connecting to WiFi ");
    Serial.println(ssid[i]);
    WiFi.begin(ssid[i], pass[i]);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 9) {
      delay(1000);
      Serial.print(".");
      attempts++;
    }
    Serial.printf("\nWiFi connected: SSID: %s, IP Address: %s\n", ssid, WiFi.localIP().toString().c_str());
  }
}

void connectUdp() {
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
}

void setupStrip(){
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            
  strip.setBrightness(255); 
}

void loop() {
  sendToTD(); 
  receiveStripDataFromTD();   
}

// send button data to TouchDesigner
void sendToTD(){
  buttonState = digitalRead(buttonPin);
  if(buttonState == prev_buttonState) return;
  prev_buttonState = buttonState;

  // check if the pushbutton is pressed. If it is, the buttonState is 1:
  Serial.println("to TD: " + String(buttonState));
  
  if (buttonState == 1) rgbLedWrite(led, 255, 255, 0);  // gelb
  else digitalWrite(led, 0);  // aus

  OSCMessage msg("/to_td");
  msg.add((int32_t)buttonState);
  Udp.beginPacket(remoteIp, remotePort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void receiveStripDataFromTD(){
  OSCBundle oscbundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      oscbundle.fill(Udp.read());
    }
    if (!oscbundle.hasError()) {
      for (int i = 0; i < oscbundle.size(); i++) {
        OSCMessage msg = oscbundle.getOSCMessage(i);
        if (strcmp(msg.getAddress(), "/colors") == 0) {
          rgbLedWrite(led, 0, 0, 255);  // turn indicator LED blue, if  OSC strip data is arriving
          int stripData = msg.getFloat(0);
          // drive strip from OSC message
          for(int i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, msg.getInt(i*3), msg.getInt((i*3)+1), msg.getInt((i*3)+2));
          }
          strip.show(); // Update strip with new contents
          delay(15);    // Pause for a moment
        } 
      }
    } else {
      OSCErrorCode error;
      error = oscbundle.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  } else digitalWrite(led, 0);  // turn off indicator LED, if no OSC strip data is arriving
}