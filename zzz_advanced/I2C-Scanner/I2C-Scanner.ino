/**********************************************************************************************
*  I2C-Scanner.ino
*  Wenn nicht klar ist, welche Adresse das angeschlossene I2C Ger$t ist, 
*  druckt dieser Code alle verfügbaren Adressen im seriellen Monitor aus.
*  4 Anschlüsse an ESP32-C6 (Kommunikation per I2C -> dafür braucht es die vorinstallierte Wire-Library):
*  Sensor: Vin  <->  ESP32-C6: 3.3V 
*  Sensor: GND  <->  ESP32-C6: GND
*  Sensor: SDA  <->  ESP32-C6: GPIO6
*  Sensor: SCL  <->  ESP32-C6: GPIO7
***********************************************************************************************/




#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial); 

  Serial.println("I2C Scanner running...");
  
  Wire.begin(6, 7);                        // SDA an GPIO6, SCL an GPIO7 
}

void loop() {
  Serial.println("Scanning...");

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {     // Antwort erhalten?
      Serial.print("I2C device found at address 0x");
      Serial.println(address, HEX);
    }
  }

  Serial.println("Scan completed.\n");
  delay(5000);                             
}
