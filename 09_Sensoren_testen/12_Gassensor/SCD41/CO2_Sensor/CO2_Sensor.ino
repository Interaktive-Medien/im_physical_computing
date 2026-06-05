/**********************************************************************************************
*  CO2_Sensor.ino
*  SCD40 / SCD41 CO₂ Sensor
*  Messe CO₂-Konzentration, Temperatur und Luftfeuchtigkeit.
*
*  Installiere die Library:
*  Sensirion I2C SCD4x (verlangt bei der Installation auch die Library Sen sidiron Core -> ebenfalls installieren)

*  4 Anschlüsse 
*  Sensor: VCC  <->  ESP32-C6: 3.3V
*  Sensor: GND  <->  ESP32-C6: GND
*  Sensor: SDA  <->  ESP32-C6: GPIO6
*  Sensor: SCL  <->  ESP32-C6: GPIO7
*
*  Typische Werte:
*    400 ppm  = Frischluft
*    800 ppm  = gut belüfteter Raum
*   1000 ppm  = durchschnittliches Büro
*   2000 ppm  = schlechte Luft
*
***********************************************************************************************/



#include <Wire.h>
#include <SensirionI2cScd4x.h>

SensirionI2cScd4x sensor;

void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7);
  sensor.begin(Wire, SCD41_I2C_ADDR_62);
  sensor.startPeriodicMeasurement();
  Serial.println("SCD4x gestartet");
}

void loop() {

  bool dataReady;
  sensor.getDataReadyStatus(dataReady);
  if (dataReady) {
    uint16_t co2;
    float temperature;
    float humidity;
    sensor.readMeasurement(co2, temperature, humidity);
    
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print(" ppm\t");

    Serial.print("Temperatur: ");
    Serial.print(temperature);
    Serial.print(" °C\t");

    Serial.print("Luftfeuchtigkeit: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
  delay(1000);
}