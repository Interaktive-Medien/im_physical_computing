/**************************************************
 * Fade_builtin_led.ino
 * Demo zur Ansteuerung der intergrierten RGB LED
 **************************************************/

void setup() {
}

void loop() {
#ifdef RGB_BUILTIN
  digitalWrite(RGB_BUILTIN, HIGH);  // Turn the RGB LED white
  delay(1000);
  digitalWrite(RGB_BUILTIN, LOW);  // Turn the RGB LED off
  delay(1000);
  for(int i = 0; i < 255; i++){
    rgbLedWrite(RGB_BUILTIN, i, 255 - i, 0);
    delay(10);
  }
  for(int i = 0; i < 255; i++){
    rgbLedWrite(RGB_BUILTIN, 255 - i, i, 0);
    delay(10);
  }
#endif
}
