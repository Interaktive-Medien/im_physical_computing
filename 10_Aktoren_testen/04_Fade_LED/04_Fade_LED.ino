/****************************************************
 *  04_Fade_LED.ino
 *  LED fadet hoch und runter
 *  Verbinde Anode (+) der LED mit GPIO 8 am ESP32-C6
 ****************************************************/

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
