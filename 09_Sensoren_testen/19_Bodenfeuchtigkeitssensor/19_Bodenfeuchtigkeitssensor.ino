/************************************************************************
 * 19_Bodenfeuchtigkeitssensor.ino
 * Lese den Feuchtigkeitssensor aus und  schreibe sie an den seriellen Port
 * Sensor: +      <->  ESP32-C6: 3.3V 
 * Sensor: -      <->  ESP32-C6: GND
 * Sensor: AOUT   <->  ESP32-C6: GPIO4
 ************************************************************************/

const int sensorPin = 4;
int sensorValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  if(sensorValue > 1500){
    Serial.println("Erde ist sehr trocken und muss bewässert werden.");
  }
  else{
    Serial.println("Erde ist feucht genug.");
  }
  delay(2000);
}