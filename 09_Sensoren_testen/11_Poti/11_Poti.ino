/************************************************************************
 * 11_Poti.ino
 * receive eg. potentiometer input and print value to serial port
 * Sensor: rechts  <->  ESP32-C6: 3.3V 
 * Sensor: links   <->  ESP32-C6: GND
 * Sensor: mitte   <->  ESP32-C6: GPIO4
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/09_Sensoren_testen/11_Poti/11_Poti.ino
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