/*************************************************************************
 * 04a_Lauflicht_Basics.ino
 * Einzelne Pixel ansteuern auf 12 Pixel WS2812B Ring
 * Installiere Library "Adafruit Neopixel" by Adafruit
 * Anschluss:
 * Idealerweise externe Stromversorgung für LEDs verwenden
 * WS2812B-Ring: Data In  <->  ESP32-C6: GPIO2
 * WS2812B-Ring: +5V/Vcc  <->  externe tromversorgung: Plusleistung
 * WS2812B-Ring: GND      <->  externe tromversorgung: Minusleistung, ESP32-C6: GND
 *************************************************************************/


#include <Adafruit_NeoPixel.h>

#define PIN 2
#define NUM_PIXELS 12
#define DELAYVAL 500

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(50);
  strip.show();                                       // standard: aus
}

void loop() {
  strip.clear();                                      // aus

  strip.setPixelColor(1, strip.Color(0, 0, 255));     // Werte: 0 - 255
  strip.show(); 

  /*
  for(int i=0; i<NUM_PIXELS; i++) {                   // für jeden einzelnen Pixel - in der Schleife
    strip.setPixelColor(i, strip.Color(0, 150, 0));   // Werte: 0 - 255
    strip.show();                                     // sende den aktualisierten Pixel an den LED-Ring
    delay(DELAYVAL);                                  // Pause vor dem nächsten Schleifendurchlauf
  }
  */
}

