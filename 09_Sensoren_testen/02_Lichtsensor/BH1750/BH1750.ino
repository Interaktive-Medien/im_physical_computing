/**********************************************************************************************
*  BH1750.ino
*  BH1750 Light Intensity Sensor
*  Messe die Beleuchtungsstärke in Lux (lx)
*  Installiere die BH1750 Library von Christopher Laws.
*  4 Anschlüsse an ESP32-C6 (Kommunikation per I2C -> dafür braucht es die vorinstallierte Wire-Library):
*  Sensor: VCC  <->  ESP32-C6: 3.3V
*  Sensor: GND  <->  ESP32-C6: GND
*  Sensor: SDA  <->  ESP32-C6: GPIO6
*  Sensor: SCL  <->  ESP32-C6: GPIO7
***********************************************************************************************/

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup() {
  Serial.begin(115200);

  Wire.begin(6, 7); // SDA, SCL

  if (lightMeter.begin()) {
    Serial.println("BH1750 gefunden");
  } else {
    Serial.println("BH1750 nicht gefunden");
    while (true);
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();

  Serial.print("Lichtstaerke: ");
  Serial.print(lux);
  Serial.println(" lx");

  delay(1000);
}