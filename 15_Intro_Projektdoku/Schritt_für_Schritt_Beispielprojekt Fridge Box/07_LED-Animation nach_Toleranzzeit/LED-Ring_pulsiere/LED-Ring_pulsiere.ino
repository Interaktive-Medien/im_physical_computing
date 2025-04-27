/*****************************************************
 * LED-Ring_pulsiere.ino
 * Lichtanimation auf 12 Pixel WS2812B Ring
 * Installiere Library "Adafruit Neopixel" by Adafruit
 * Anschluss:
 * Idealerweise externe Stromversorgung für LEDs verwenden
 * WS2812B-Ring: Data In  <->  ESP32-C6: GPIO2
 * WS2812B-Ring: +5V/Vcc  <->  externe tromversorgung: Plusleistung
 * WS2812B-Ring: GND      <->  externe tromversorgung: Minusleistung, ESP32-C6: GND
  *****************************************************/


#include <Adafruit_NeoPixel.h>
#include <math.h> // für pow()

#define LEDRING_PIN 2
#define NUM_LEDRING_PIXELS 12

Adafruit_NeoPixel ledring = Adafruit_NeoPixel(NUM_LEDRING_PIXELS, LEDRING_PIN, NEO_GRB + NEO_KHZ800);

// Variablen für non-blocking Timing
unsigned long ledring_previousMillis = 0;
const long ledring_interval = 70; // kürzerer Schritt für weiches Pulsieren

// Variablen für Puls-Animation
int ledring_pulse_helligkeit = 0; // 0-255 linear
int ledring_pulse_richtung = 1; // 1 = heller werden, -1 = dunkler werden

void setup() {
  ledring.begin();
  ledring.setBrightness(100); // Grundhelligkeit
  ledring.show(); // Alles aus
}

void loop() {
  pulsiere_blau();
}

void pulsiere_blau() {
  unsigned long currentMillis = millis();

  if (currentMillis - ledring_previousMillis >= ledring_interval) {
    ledring_previousMillis = currentMillis;

    // Lineare Helligkeit anpassen
    ledring_pulse_helligkeit += ledring_pulse_richtung * 5;

    // Grenzen prüfen und Richtung umkehren
    if (ledring_pulse_helligkeit >= 255) {
      ledring_pulse_helligkeit = 255;
      ledring_pulse_richtung = -1;
    } else if (ledring_pulse_helligkeit <= 0) {
      ledring_pulse_helligkeit = 0;
      ledring_pulse_richtung = 1;
    }

    // Gammakorrektur anwenden (hier Gamma ≈ 2.2)
    float normalized = ledring_pulse_helligkeit / 255.0;
    float gammaCorrected = pow(normalized, 2.2); // Wertebereich bleibt 0.0 - 1.0

    // Minimalwert für die Farbe definieren (z.B. 20% vom Maximalwert)
    const float MIN_B = 0.06;  // 5% Blau bei minimaler Helligkeit
    const float MIN_G = 0.0;  // kein Grün bei minimaler Helligkeit

    // Farbwerte zwischen MIN und MAX skalieren
    float g_float = MIN_G + (0.7 - MIN_G) * gammaCorrected; // Grünanteil skaliert
    float b_float = MIN_B + (1.0 - MIN_B) * gammaCorrected; // Blauanteil skaliert

    uint8_t r = 0;
    uint8_t g = (uint8_t)(g_float * 255.0);
    uint8_t b = (uint8_t)(b_float * 255.0);

    // LEDs setzen
    for (int i = 0; i< NUM_LEDRING_PIXELS; i++) {
      ledring.setPixelColor(i, ledring.Color(r, g, b));
    }

    ledring.show();
  }
}
