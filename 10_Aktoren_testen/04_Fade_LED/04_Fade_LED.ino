/****************************************************
 *  04_Fade_LED.ino
 *  LED fadet auf und ab
 *  Verbinde:
 *  LED: + (langes Bein)  <->  ESP32-C6: GPIO 8 (per Vorwiederstand)
 *  LED: - (kurzes Bein)  <->  ESP32-C6: GND 
 *  GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/10_Aktoren_testen/04_Fade_LED/04_Fade_LED.ino
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
