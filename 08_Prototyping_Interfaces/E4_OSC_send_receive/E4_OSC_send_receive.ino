// Button signal to TouchDesigner and back via OSC
// load library "OSC" by Adrian Freed
// specify your Wifi ssid and pw, and IP address of your TouchDesigner PC

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

const char* ssid[] = {"tinkergarden", "Igloo"};   // @todo: add your wifi name
const char* pass[] = {"strenggeheim", "1glooVision"};  // @todo: add your wifi pw

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(172, 20, 10, 8);      // @todo: add receiver IP address
const unsigned int remotePort = 10000;      // This is the default port of the oscinChop in TouchDesigner
const unsigned int localPort = 8888;        // Local port to listen for OSC packets (actually not used for sending)

const int buttonPin = 7;                   // GPIO13 for button input
int buttonState = 0;         
int prev_buttonState = 0;
const int led =  RGB_BUILTIN;                        

void setup() {
  Serial.begin(115200);
  connectWiFi();
  connectUdp();

  pinMode(buttonPin, INPUT);  
  pinMode(led, OUTPUT);    
  digitalWrite(led, 0);
}

void connectWiFi() {
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

void loop() {
  sendToTD(); 
  receiveFromTD();   
}

// Send button data to TouchDesigner
void sendToTD() {
  buttonState = digitalRead(buttonPin);
  if(buttonState == prev_buttonState) return;
  prev_buttonState = buttonState;

  // Check if the pushbutton is pressed. If it is, the buttonState is 1:
  Serial.println("to TD: " + String(buttonState));
  
  if (buttonState == 1) rgbLedWrite(led, 255, 255, 0);  // gelb
  else digitalWrite(led, 0);  // aus

  OSCMessage msg("/to_td");
  msg.add((int32_t)buttonState);   // Explicitly cast buttonState to int32_t
  Udp.beginPacket(outIp, remotePort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
  delay(500);
}

void receiveFromTD() {
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {
      for (int i = 0; i < bundle.size(); i++) {
        OSCMessage msg = bundle.getOSCMessage(i);
        if (strcmp(msg.getAddress(), "/from_td") == 0) {
          int val = msg.getFloat(0);
          if (val == 1) rgbLedWrite(led, 0, 0, 255);  // blau
          else digitalWrite(led, 0);  // turn off

          Serial.print("/from_td: ");
          Serial.println(val);
        }
      }
    } else {
      OSCErrorCode error;
      error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
