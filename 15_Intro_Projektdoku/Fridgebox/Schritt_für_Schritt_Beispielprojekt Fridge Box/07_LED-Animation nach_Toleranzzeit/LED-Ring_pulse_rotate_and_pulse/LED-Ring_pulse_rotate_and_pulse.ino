/*****************************************************
 * LED-Ring_pulse_rotate_and_pulse.ino
 * Lichtanimation auf 12 Pixel WS2812B Ring
 * Installiere Library "Adafruit Neopixel" by Adafruit
 * Anschluss:
 * Idealerweise externe Stromversorgung für LEDs verwenden
 * WS2812B-Ring: Data In  <->  ESP32-C6: GPIO2
 * WS2812B-Ring: +5V/Vcc  <->  externe Stromversorgung: Plusleistung
 * WS2812B-Ring: GND      <->  externe Stromversorgung: Minusleistung, ESP32-C6: GND

 * wechsle zwischen Anims, indem du 0, 1 oder 2 an die serielle Schnittstelle des MC schreibst.
 *****************************************************/


#include <Adafruit_NeoPixel.h>
#include <math.h> // für pow()

#define LEDRING_PIN 2
#define NUM_LEDRING_PIXELS 12

Adafruit_NeoPixel ledring = Adafruit_NeoPixel(NUM_LEDRING_PIXELS, LEDRING_PIN, NEO_GRB + NEO_KHZ800);

// Variablen für non-blocking Timing
unsigned long ledring_previous_update_timestamp = 0;

// Variablen für Lauflicht
int ledring_center = 0;
const long ledring_lauflicht_interval = 80; // Lauflicht Schrittzeit

// Variablen für Puls-Animation
int ledring_pulse_helligkeit = 0; // 0-255 linear
int ledring_pulse_richtung = 1; // 1 = heller werden, -1 = dunkler werden
const long ledring_pulse_interval = 70;     // Pulsier Schrittzeit

// Aktuelle Modus-Variable
int ledring_modus = 0; // 0 = aus, 1 = Pulsieren, 2 = rotes Lauflicht

void setup() {
  ledring.begin();
  ledring.setBrightness(100); // Grundhelligkeit (0 - 255)

  Serial.begin(115200); // Seriell starten für Steuerung
}

void loop() {
  // Seriellen Eingang prüfen
  if (Serial.available() > 0) {
    char eingabe = Serial.read();
    if (eingabe == '0') {
      ledring_modus = 0;
    } else if (eingabe == '1') {
      ledring_modus = 1;
    } else if (eingabe == '2') {
      ledring_modus = 2;
    }
  }

  // Entsprechend Modus Animation aufrufen
  if (ledring_modus == 0) {
    ledring_aus();  // LED-Ring ausschalten
  } else if (ledring_modus == 1) {
    ledring_pulsiere_blau();
  } else if (ledring_modus == 2) {
    ledring_rotiere_rotes_lauflicht();
  }
}

// Funktion für den ausgeschalteten LED-Ring (komplett schwarz)
void ledring_aus() {
  for (int i = 0; i < NUM_LEDRING_PIXELS; i++) {
    ledring.setPixelColor(i, ledring.Color(0, 0, 0)); // Setzt alle LEDs auf Schwarz
  }
  ledring.show(); // Anwenden
}

void ledring_rotiere_rotes_lauflicht() {
  unsigned long currentMillis = millis();

  if (currentMillis - ledring_previous_update_timestamp >= ledring_lauflicht_interval) {
    ledring_previous_update_timestamp = currentMillis;

    ledring.clear();

    // Sanfter Verlauf über 7 LEDs: -3, -2, -1, 0, +1, +2, +3
    for (int offset = -3; offset <= 3; offset++) {
      int pixel = (ledring_center + offset + NUM_LEDRING_PIXELS) % NUM_LEDRING_PIXELS; // wrap-around Ring

      uint8_t brightnessFactor;
      switch (offset) {
        case 0:  brightnessFactor = 255; break; // Zentrum voll hell
        case -1: case 1: brightnessFactor = 110; break; // erste Nachbarn mittelhell
        case -2: case 2: brightnessFactor = 25; break; // äußere Nachbarn schwach
        case -3: case 3: brightnessFactor = 7; break; // äußerste schwach
      }
      
      ledring.setPixelColor(pixel, ledring.Color(brightnessFactor, 0, 0)); // Rot
    }

    ledring.show();

    ledring_center = (ledring_center + 1) % NUM_LEDRING_PIXELS; // einen Schritt weiter
  }
}

void ledring_pulsiere_blau() {
  unsigned long currentMillis = millis();

  if (currentMillis - ledring_previous_update_timestamp >= ledring_pulse_interval) {
    ledring_previous_update_timestamp = currentMillis;

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

    // Gammakorrektur anwenden (Gamma ≈ 2.2)
    float normalized = ledring_pulse_helligkeit / 255.0;
    float gammaCorrected = pow(normalized, 2.2); // Wertebereich 0.0 - 1.0

    // Minimalwerte für dunkles Blau definieren
    const float MIN_B = 0.06;  // 6% Blau
    const float MIN_G = 0.0;   // 0% Grün

    // Farbwerte skalieren
    float g_float = MIN_G + (0.7 - MIN_G) * gammaCorrected;
    float b_float = MIN_B + (1.0 - MIN_B) * gammaCorrected;

    uint8_t r = 0;
    uint8_t g = (uint8_t)(g_float * 255.0);
    uint8_t b = (uint8_t)(b_float * 255.0);

    // LEDs setzen
    for (int i = 0; i < NUM_LEDRING_PIXELS; i++) {
      ledring.setPixelColor(i, ledring.Color(r, g, b));
    }

    ledring.show();
  }
}
