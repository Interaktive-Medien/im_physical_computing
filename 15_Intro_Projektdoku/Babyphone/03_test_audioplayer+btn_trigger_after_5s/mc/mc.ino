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

int timeUntilAlert = 5000;
unsigned long pressStartTime = 0;
bool fired = false;

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
  buttonState = digitalRead(buttonPin);

  // Fall 1: Knopf wurde gerade erst gedrückt
  if (buttonState == HIGH && prev_buttonState == LOW) {
    pressStartTime = millis();  // Startzeit merken
    fired = false;              // Reset für diesen Tastendruck
    digitalWrite(ledPin, HIGH); // LED sofort an, um Feedback zu geben
  }

  // Fall 2: Knopf wird gehalten
  if (buttonState == HIGH && !fired) {
    if (millis() - pressStartTime >= timeUntilAlert) { // 5000ms = 5s
      Serial.println("fire");
      playTrack(1);
      fired = true;             // Markieren, dass wir gefeuert haben
    }
  }

  // Fall 3: Knopf wurde losgelassen
  if (buttonState == LOW && prev_buttonState == HIGH) {
    digitalWrite(ledPin, LOW);  // LED aus
  }

  prev_buttonState = buttonState;
}

