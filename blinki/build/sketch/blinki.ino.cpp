#include <Arduino.h>
#line 1 "G:\\Meine Ablage\\FHGR\\Lehre\\IM\\IM4\\FS25\\Physical Computing\\im_physical_computing\\blinki\\blinki.ino"
#line 1 "G:\\Meine Ablage\\FHGR\\Lehre\\IM\\IM4\\FS25\\Physical Computing\\im_physical_computing\\blinki\\blinki.ino"
void setup();
#line 7 "G:\\Meine Ablage\\FHGR\\Lehre\\IM\\IM4\\FS25\\Physical Computing\\im_physical_computing\\blinki\\blinki.ino"
void loop();
#line 1 "G:\\Meine Ablage\\FHGR\\Lehre\\IM\\IM4\\FS25\\Physical Computing\\im_physical_computing\\blinki\\blinki.ino"
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off (LOW is the voltage level)
  delay(1000);                      // wait for a second
}

