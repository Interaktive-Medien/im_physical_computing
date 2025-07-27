/*******************************************
 * BNO055: Stabiler Lagesensor ohne Drift
 * Tutorial: https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code
 * hier zu kaufen: https://www.maker-shop.ch/adafruit-9-dof-absolute-orientation-imu-fusion-breakout-bno055
 *******************************************/


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// BNO055 I2C-Adresse ist standardmäßig 0x28, die 55 im Konstruktor ist der Sensor-ID (nicht I2C-Adresse)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);  // <– explizit Wire übergeben

void setup(void) 
{
  Serial.begin(115200);
  delay(100);

  // ⬅️ ESP32-C6 I2C Pins setzen
  Wire.begin(6, 7);

  Serial.println("Orientation Sensor Test");
  
  // ⬅️ BNO initialisieren
  if(!bno.begin())
  {
    Serial.println("❌ Kein BNO055 gefunden. Bitte Verdrahtung prüfen!");
    while(1);
  }

  delay(1000);
  bno.setExtCrystalUse(true);  // für bessere Stabilität
}

void loop(void) 
{
  sensors_event_t event;
  bno.getEvent(&event);

  Serial.print("Roll (X): ");
  Serial.print(event.orientation.x, 1);
  Serial.print("°\tPitch (Y): ");
  Serial.print(event.orientation.y, 1);
  Serial.print("°\tHeading (Z): ");
  Serial.print(event.orientation.z, 1);
  Serial.println("°");

  delay(100);
}
