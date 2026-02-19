/******************************************************************************************************
 * mc.ino
 * control Open-Smart audio player with ESP32-C6
 * Connect ...
 * audio player: Vin  <->  ESP32-C6: 5V 
 * audio player: GND  <->  ESP32-C6: GND  (per npn transistor)
 * audio player: RX   <->  ESP32-C6: GPIO7
 * audio player: TX   <->  ESP32-C6: GPIO6
 * basis transistor   <->  ESP32-C6: GPIO5
 * button pin 1       <->  ESP32-C6: GPIO15
 * button pin 2       <->  ESP32-C6: 3V3
 ******************************************************************************************************/



#include "audioplayer.h"

// button
const int buttonPin = 15;
const int ledPin =  BUILTIN_LED; 
int buttonState = 0;         
int prev_buttonState = 0;

void setup() {
  Serial.begin(115200);
  initAudioPlayer();

  // button
  pinMode(buttonPin, INPUT_PULLDOWN);  // initialize the pushbutton pin as an input:
  pinMode(ledPin, OUTPUT);             // initialize the LED pin as output:
  digitalWrite(ledPin, 0);             // turn LED off:

  // led
  pinMode(ledPin, OUTPUT);             // initialize the LED pin as output:
  digitalWrite(ledPin, 0);             // turn LED off:
}

void loop() {
  // button
  buttonState = digitalRead(buttonPin);
  if(buttonState == prev_buttonState) {
      return;
  }

  prev_buttonState = buttonState;
  if (buttonState == 1) {
      playTrack(1);
      digitalWrite(ledPin, 1);          // turn LED on:
      Serial.println(3);
  } else {
      playTrack(2);
      digitalWrite(ledPin, 0);          // turn LED off:
      Serial.println(4);
  } 
}

