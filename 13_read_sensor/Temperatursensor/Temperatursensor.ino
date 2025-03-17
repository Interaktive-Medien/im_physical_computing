/********************************************************************
 * Temperatursensor.ino
 * receive sensor input from DHT11 and print value to serial port
 * Installiere "DHT sensor library" by Adafruit
 *  Anschluss an ESP32-C6
 *  Sensor: Vin  <->  ESP32-C6: 3.3V 
 *  Sensor: GND  <->  ESP32-C6: GND
 *  Sensor: DATA <->  ESP32-C6: GPIO7
 ********************************************************************/


#include "DHT.h"

#define DHTPIN 7    
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // °C

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
}
