/************************************************************************
 * 12_Gassensor_average.ino
 * gib Durchschnittswerte der letzten Werte aus, um das Signal zu glätten
 * Sensor: AO   <->  ESP32-C6: 4 
 * Sensor: VCC  <->  ESP32-C6: 5V 
 * Sensor: GND  <->  ESP32-C6: GND
 * Für bessere Ergebnisse und mehr Langlebigkeit des Microcontrollers 
 * sollte das Datensignal des Gas-Sensors (max. 5V) mit einem Level Shifter
 * oder einem Spannungsteiler auf den Wertebereich 0...3.3V heruntergeregelt werden.   
 ************************************************************************/

const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
int sensorpin = 4;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  total -= readings[readIndex];
  readings[readIndex] = analogRead(sensorpin);
  total += readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;

  average = total / numReadings;
  Serial.println(average);
  delay(100);
}
