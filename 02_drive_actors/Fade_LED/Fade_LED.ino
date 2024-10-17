// Fade_LED.ino

int led = 8;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  for(int i = 0; i < 255; i++){
    analogWrite(led, i);
    delay(10);
  }
  for(int i = 0; i < 255; i++){
    analogWrite(led, 255 - i);
    delay(10);
  }
}
