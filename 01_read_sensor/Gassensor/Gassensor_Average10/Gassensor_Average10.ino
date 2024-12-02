const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
int sensorpin = 0;

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
