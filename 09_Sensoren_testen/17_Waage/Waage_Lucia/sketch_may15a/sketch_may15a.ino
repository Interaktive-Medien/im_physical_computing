// =========================== Bibliotheken einbinden ===============================
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "HX711.h"
#include <Adafruit_NeoPixel.h>

// =========================== WLAN-Zugangsdaten ===============================
const char* ssid     = "tinkergarden";
const char* pass     = "strenggeheim";

// =========================== Server-URL ===============================
const char* serverURL = "https://im4.luciaschnyder.ch/load.php";

// =========================== Waage Setup ===============================
#define HX711_DT  6
#define HX711_SCK 7
HX711 scale;
float calibration_factor = -195.72;

// =========================== LED Setup ===============================
#define LED_PIN 2
#define NUM_PIXELS 12
#define BRIGHTNESS 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// =========================== Schwellen ===============================
float gewichtGrenze = 500.0;
float detectionThreshold = 0.1;  // Ab wann wird Flasche erkannt (> 0.1g)
int minValidMessungen = 5;        // Mindestanzahl an Messwerten für gültige Messung

// =========================== Flaschen-Logik ===============================
bool flascheErkannt = false;
unsigned long letzteMessung = 0;
const unsigned long checkIntervall = 1000;  // alle 1 Sekunde prüfen

// Für Mittelwertberechnung
float gewichtSumme = 0.0;
int anzahlMesswerte = 0;

// =========================== Zusätzliche Variablen für LED-Erinnerung ===============================
bool ledErinnerungAktiv = false;
unsigned long flascheSeit = 0;
const unsigned long LED_ERINNERUNG_DAUER = 30 * 1000UL;  // 30 Sekunden

// =========================== SETUP ===============================
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.print("Verbinde mit WLAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWLAN verbunden!");
  Serial.println(WiFi.localIP());

  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(calibration_factor);
  scale.tare();
  Serial.println("Waage bereit!");

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

// =========================== LOOP ===============================
void loop() {
  unsigned long jetzt = millis();

  if (jetzt - letzteMessung >= checkIntervall) {
    letzteMessung = jetzt;

    float gewicht = scale.get_units(3);  // 3 Messwerte mitteln
    Serial.print("Aktuelles Gewicht: ");
    Serial.println(gewicht);

    // Wenn Flasche erkannt → mit Messung beginnen
    if (gewicht > detectionThreshold) {
      if (!flascheErkannt) {
        Serial.println("Flasche erkannt – Messung beginnt...");
        flascheErkannt = true;
        gewichtSumme = 0.0;
        anzahlMesswerte = 0;
      }

      gewichtSumme += gewicht;
      anzahlMesswerte++;
    }

    // Flasche wurde entfernt → prüfen ob genug Daten da sind
    if (flascheErkannt && gewicht < detectionThreshold) {
      Serial.println("Flasche entfernt – prüfe Daten...");

      if (anzahlMesswerte > minValidMessungen) {
        float durchschnitt = gewichtSumme / anzahlMesswerte;
        Serial.print("Durchschnittsgewicht: ");
        Serial.println(durchschnitt);

        // Senden an Server
        if (WiFi.status() == WL_CONNECTED) {
          JSONVar data;
          data["wert"] = durchschnitt;
          String jsonString = JSON.stringify(data);

          HTTPClient http;
          http.begin(serverURL);
          http.addHeader("Content-Type", "application/json");
          int responseCode = http.POST(jsonString);

          if (responseCode > 0) {
            String response = http.getString();
            Serial.print("Server-Antwort: ");
            Serial.println(response);
          } else {
            Serial.print("Fehler beim Senden. Code: ");
            Serial.println(responseCode);
          }
          http.end();
        }
      } else {
        Serial.println("Nicht genug Messwerte – ignoriere.");
      }

      // System zurücksetzen
      flascheErkannt = false;
      gewichtSumme = 0.0;
      anzahlMesswerte = 0;
    }

    // =========================== LED-Erinnerungslogik ===============================
    if (gewicht > detectionThreshold) {
      if (flascheSeit == 0) {
        flascheSeit = jetzt;
      }

      if (!ledErinnerungAktiv && (jetzt - flascheSeit >= LED_ERINNERUNG_DAUER)) {
        Serial.println("Flasche > 30 Sekunden erkannt – LED-Ring aktivieren!");
        ledErinnerungAktiv = true;

        for (int i = 0; i < NUM_PIXELS; i++) {
          strip.setPixelColor(i, strip.Color(0, 0, 255)); // blau
        }
        strip.show();
      }
  } else if (gewicht < detectionThreshold - 0.1) {  // Etwas Spielraum nach unten
  if (ledErinnerungAktiv) {
    Serial.println("Flasche entfernt – LED-Ring deaktivieren.");
    ledErinnerungAktiv = false;
    strip.clear();
    strip.show();
  }
  flascheSeit = 0;
}
  }
}