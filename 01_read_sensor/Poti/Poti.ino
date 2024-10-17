/************************************************************************
 * Poti.ino
 * receive eg.  potentiometer input and print value to serial port
 ************************************************************************/

const int potPin = 4;
int potValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(500);
}