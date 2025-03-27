#include <FastLED.h>
 
#define LED_PIN     2
#define NUM_LEDS    12
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];   // Array zur Speicherung des LED-Zustands
 
#define UPDATES_PER_SECOND 100   // (Steuerung der Animationsgeschwindigkeit)
 
// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.
 
 
// Farbpalette und Blending-Typ für die LED-Anzeige
CRGBPalette16 currentPalette; // hier werden Farben (in der Funktion ChangePalettePeriodically) und im Programm gewechselt.
TBlendType    currentBlending;

// Deklaration einer externen Farbpalette (rot-weiß-blau) -> extern: woanders im Code, progmem: gespeichert im flash, nicht im RAM.
extern const TProgmemPalette16 myRedWhiteBluePalette_p FL_PROGMEM;
 
// If you are using the fastled compiler, then you must declare your functions
// before you use them. This is standard in C++ and C projects, but ino's are
// special in that they do this for you. Eventually we will try to emulate this
// feature ourselves but in the meantime you'll have to declare your functions
// before you use them if you want to use our compiler.

// Funktionsprototypen (notwendig für den Compiler) -> you'll have to declare your functions before you use them if you want to use our compiler.
void ChangePalettePeriodically();
void FillLEDsFromPaletteColors(uint8_t colorIndex);
void SetupPurpleAndGreenPalette();   // spezifisch für Farbpalette
void SetupBlackAndWhiteStripedPalette();   // spezifisch für Farbpalette
 
 
void setup() {
    delay( 3000 ); // Sicherheitsverzögerung beim Start
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    currentBlending = LINEARBLEND;
    FastLED.clear ();

    Serial.begin(115200);
}
 
 
void loop()
{
    ChangePalettePeriodically(15); // Ändert alle 15 Sekunden die Farbpalette
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; // Steuerung der Animation (grösserer Wert: schneller / kleinerer Wert: langsamer)
    
    // FillLEDsFromPaletteColors_runninglight( startIndex);
    FillLEDsFromPaletteColors_alltogether( startIndex);
    
    FastLED.show();   // LED-Daten senden
    FastLED.delay(1000 / UPDATES_PER_SECOND); // Animationstempo regeln
}
 

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.


void ChangePalettePeriodically(int dauer){
    static CRGBPalette16 targetPalette;
    static uint8_t lastSecond = 99;
    uint8_t uhr = (millis() / 1000) % 45; // zählt hoch von 0 - 44 und beginnt wieder bei 0

    if (lastSecond != uhr) {
        lastSecond = uhr;

        if( uhr ==  0 * dauer)  { 
            Serial.println("anim 1");
            SetupPurpleAndGreenPalette();
            targetPalette = currentPalette;
        }
        if( uhr ==  1 * dauer)  { 
            Serial.println("anim 2");
            SetupBlackAndWhiteStripedPalette();
            targetPalette = currentPalette;
        }
        if( uhr ==  2 * dauer)  { 
            Serial.println("anim 3");
            targetPalette = myRedWhiteBluePalette_p;
        }
    }

    // Überblendung zwischen den Paletten in 48 Schritten (~3 Sekunden)
    nblendPaletteTowardPalette(currentPalette, targetPalette, 48);
}



// Funktion, um LEDs aus der aktuellen Farbpalette zu füllen
void FillLEDsFromPaletteColors_runninglight( uint8_t colorIndex){
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void FillLEDsFromPaletteColors_alltogether(uint8_t colorIndex) {
    uint8_t brightness = 255;
    CRGB color = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    
    for (int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = color;  // Alle LEDs bekommen die gleiche Farbe
    }
}
 
 



// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  green,  green,
                                   green, green, green,  green,
                                   purple,  purple,  purple,  purple,
                                   purple, purple, purple,  purple );
}
 
// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[1] = CRGB::White;
    currentPalette[2] = CRGB::White;
    currentPalette[3] = CRGB::White;
    
}
 

 
// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p FL_PROGMEM =
{
    CRGB::Red,
    CRGB::Red, 
    CRGB::Red,
    CRGB::Red,
    
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,
    
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,

    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue
};
 
