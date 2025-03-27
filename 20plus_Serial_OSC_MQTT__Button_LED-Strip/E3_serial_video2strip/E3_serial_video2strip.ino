/*******************************************************************************
 * Empfange Lichtdaten per serieller Kommunikation von z. B. TouchDesigner
 * installiere Library "Adafruit NeoPixel" by Adafruit
 * Verbinde 12 WS2812B LEDs (z. B. LED-Ring) mit ESP32-C6:
 * WS2812B: Data in (Di)  <->  ESP32-C6: GPIO 2
 * WS2812B: 5V            <->  5V (Externe Stromversorgung, falls zu viele LEDs)
 * WS2812B: GND           <->  GND (Externe Stromversorgung, falls zu viele LEDs) 
 ********************************************************************************/


#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

int led_pin = 2; 
const int numpixels = 12; 

Adafruit_NeoPixel pixels(numpixels, led_pin, NEO_GRB + NEO_KHZ800);
char inputBuffer[numpixels * 3];

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  
  pixels.begin();
  pixels.show();
  pixels.setBrightness(255);
  Serial.begin(115200);
  Serial.setTimeout(500);
}

void loop() {
  if(Serial.available() > 0){
    Serial.readBytes(inputBuffer, numpixels * 3); // inputBuffer: Zielarray, nuppixels * 3: Anzahl der Werte, die aus dem seriellen Buffer gelesen werden sollen
  }

  for(int i=0; i < numpixels; i++){
    pixels.setPixelColor(i, inputBuffer[(i*3)], inputBuffer[(i*3)+1], inputBuffer[(i*3)+2]);  
    //pixels.setPixelColor(i, 0,0,255);  // blau zum Testen ob LEDs funktionieren
  }
  pixels.show();
  delay(11);
}