/*****************************************************
 * 04b_Lauflicht_Anim.ino
 * Lichtanimation auf 12 Pixel WS2812B Ring
 * Installiere Library "Adafruit Neopixel" by Adafruit
 * Anschluss:
 * Idealerweise externe Stromversorgung für LEDs verwenden
 * WS2812B-Ring: Data In  <->  ESP32-C6: GPIO2
 * WS2812B-Ring: +5V/Vcc  <->  externe tromversorgung: Plusleistung
 * WS2812B-Ring: GND      <->  externe tromversorgung: Minusleistung, ESP32-C6: GND
 *****************************************************/


#include <Adafruit_NeoPixel.h>

#define PIN 2
#define NUM_PIXELS 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  rainbowCycle(20);
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
