/********************************************************************
 * 08_Temperatursensor.ino
 * receive sensor input from DHT11 and print value to serial port
 * Installiere "DHT sensor library" by Adafruit
 * Anschluss an ESP32-C6:
 * Sensor: Vin  <->  ESP32-C6: 3.3V 
 * Sensor: GND  <->  ESP32-C6: GND
 * Sensor: DATA <->  ESP32-C6: GPIO7
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/09_Sensoren_testen/08_Temperatursensor/08_Temperatursensor.ino
 ********************************************************************/

#include "DHT.h"
#define DHTPIN 7    
#define DHTTYPE DHT11           
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();       // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();    // °C

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);    // heat in Celsius
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
}
