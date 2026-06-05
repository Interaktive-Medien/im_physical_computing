/**********************************************************************************************
* MQ2_Sensor.ino
* MQ-2 Gas- und Rauchsensor
* Messe die Gaskonzentration (Rauch, LPG, Propan) als analogen Rohwert.
*
* Keine externe Library erforderlich.
* 4 Anschlüsse
* Sensor: VCC <-> ESP32-C6: 5V (Wichtig: Benötigt 5V für das Heizelement!)
* Sensor: GND <-> ESP32-C6: GND
* Sensor: AO  <-> ESP32-C6: GPIO0 (Analoger Eingang)
* Sensor: DO  <-> Nicht verbunden (Digitaler Schwellenwert-Ausgang, hier ignoriert)
*
* Typische Werte (bei 12-Bit ADC Auflösung 0-4095):
* < 500   = Saubere Raumluft
* 500-1200 = Leichte Luftverunreinigung / Erwärmungsphase
* > 1200  = Möglicher Gas- oder Rauchalarm
*
***********************************************************************************************/

#define MQ2_ANALOG_PIN 0

void setup() {
  Serial.begin(115200);
  // Setze die ADC-Auflösung auf 12-Bit (0-4095), passend für ESP32
  analogReadResolution(12); 
  Serial.println("MQ-2 Sensor gestartet (Aufwärmphase beachten)");
}

void loop() {
  // Analogen Rohwert einlesen
  int gasValue = analogRead(MQ2_ANALOG_PIN);
  
  Serial.print("Gas/Rauch Analogwert: ");
  Serial.println(gasValue);
  
  delay(1000);
}
