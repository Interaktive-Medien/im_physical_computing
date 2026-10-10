/*****************************************************
 * LED-Ring_rotiere.ino
 * Lichtanimation auf 12 Pixel WS2812B Ring
 * Installiere Library "Adafruit Neopixel" by Adafruit
 * Anschluss:
 * Idealerweise externe Stromversorgung für LEDs verwenden
 * WS2812B-Ring: Data In  <->  ESP32-C6: GPIO2
 * WS2812B-Ring: +5V/Vcc  <->  externe tromversorgung: Plusleistung
 * WS2812B-Ring: GND      <->  externe tromversorgung: Minusleistung, ESP32-C6: GND
  *****************************************************/

#include <Adafruit_NeoPixel.h>

#define LEDRING_PIN 2
#define NUM_LEDRING_PIXELS 12

Adafruit_NeoPixel ledring = Adafruit_NeoPixel(NUM_LEDRING_PIXELS, LEDRING_PIN, NEO_GRB + NEO_KHZ800);

// Variablen für non-blocking Timing
unsigned long ledring_previousMillis = 0;
const long ledring_interval = 80; // alle 80ms ein Schritt -> langsamer

int ledring_center = 0; // aktuelle Mittel-LED

void setup() {
  ledring.begin();
  ledring.setBrightness(100); // Gesamthelligkeit
  ledring.show(); // Alles aus
}

void loop() {
  rotiere_rotes_lauflicht();
}

void rotiere_rotes_lauflicht() {
  unsigned long currentMillis = millis();

  if (currentMillis - ledring_previousMillis >= ledring_interval) {
    ledring_previousMillis = currentMillis;

    ledring.clear();

    // Sanfter Verlauf über 7 LEDs: -3, -2, -1, 0, +1, +2, +3
    for (int offset = -3; offset <= 3; offset++) {
      int pixel = (ledring_center + offset + NUM_LEDRING_PIXELS) % NUM_LEDRING_PIXELS; // wrap-around Ring

      uint8_t brightnessFactor;
      switch (offset) {
        case 0:  brightnessFactor = 255; break; // Zentrum voll hell
        case -1: case 1: brightnessFactor = 110; break; // erste Nachbarn mittelhell
        case -2: case 2: brightnessFactor = 25; break; // äußere Nachbarn schwach
        case -3: case 3: brightnessFactor = 7; break; // äußere Nachbarn schwach
      }
      
      ledring.setPixelColor(pixel, ledring.Color(brightnessFactor, 0, 0)); // Rot mit angepasster Helligkeit
    }

    ledring.show();

    ledring_center = (ledring_center + 1) % NUM_LEDRING_PIXELS; // Einen Schritt weiter
  }
}
