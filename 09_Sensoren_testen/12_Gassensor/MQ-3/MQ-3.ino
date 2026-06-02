/**********************************************************************************************
*  MQ-3.ino
*  MQ-3 Alcohol Gas Sensor
*  Messe die Konzentration von Alkohol-Dämpfen über den analogen Ausgang (A0)
*  Der Sensor liefert einen analogen Messwert, der je nach Alkoholkonzentration steigt.
*  Die Werte sind ohne Kalibrierung nicht direkt als Promille oder mg/L interpretierbar.
*  Es wird keine zusätzliche Library benötigt.
*
*  Anschlüsse an ESP32-C6:
*  Sensor: VCC  <->  ESP32-C6: 5V
*  Sensor: GND  <->  ESP32-C6: GND
*  Sensor: A0   <->  ESP32-C6: GPIO5 (beliebiger ADC-Pin möglich)
*  Sensor: D0   <->  nicht angeschlossen
*
*  Hinweis:
*  Der MQ-3 enthält eine Heizung und benötigt einige Minuten Aufwärmzeit.
*  Für stabile Messungen sollte der Sensor ca. 1-5 Minuten vorheizen.
***********************************************************************************************/

const int MQ3_PIN = 5;  // GPIO5

void setup() {
  Serial.begin(115200);
}

void loop() {

  int adc = analogRead(MQ3_PIN);

  Serial.print("MQ-3 ADC: ");
  Serial.println(adc);

  delay(500);
}