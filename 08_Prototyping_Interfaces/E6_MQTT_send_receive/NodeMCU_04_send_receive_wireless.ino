// Install Library "MQTT" by Joel Gaehwiler
// Install Library "Adafruit NeoPixel"


#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <Adafruit_NeoPixel.h>


#define PIN       15 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 30
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


const char ssid[] = "dreammakers";
const char pass[] = "dreammakers";
const char broker[] = "127.0.0.1";

WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

// button
const int buttonPin = 13;

int buttonState = 0;         
int prev_buttonState = 0;
const int ledPin =  BUILTIN_LED;    // BUILTIN_LED == 2

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("fromtd");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin(broker, net);  //test.mosquitto.org -> über Internet
  client.onMessage(messageReceived);

  connect();


  // button 
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);   
  // set the LED pin as output:
  pinMode(ledPin, OUTPUT);



  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)


}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

/*
  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("bubble", "test");
  }
*/

  // button

  receiveFromTD();
  sendToTD();

  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}

void receiveFromTD(){
  return;
}



// send button data to TouchDesigner
void sendToTD(){
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == prev_buttonState) {
    return;
  }

  prev_buttonState = buttonState;

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, buttonState);
    Serial.println(buttonState);
    client.publish("bubble", "1");
  } else {
    // turn LED off:
    digitalWrite(ledPin, buttonState);
    Serial.println(buttonState);
    client.publish("bubble", "0");
  }  
}