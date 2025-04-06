/***********************************************************************************************************************
 * LED-Animationen mit FastLED, Jan Fiess 2025
 * Dieses Beispielprogramm zeigt 3 Animationsmöglichkeiten:
 * - fade_to_color: Überblendung auf die Zielfarbe mit einstellbarer Geschwindigkeit.
 * - running_lights: Lauflicht, basierend auf Farbpalette, im Loop, mit einstellbarer Geschwindigkeit, Ausdehnung der Anim
 * - all_equally: Alle LEDs machen dasselbe, basierend auf Farbpalette, im Loop. mit einstellbarer Geschwindigkeit
 * Animationen können im seriellen Monitor mit den Tasten 1, ... getriggertwerden
 ***********************************************************************************************************************/


#include <FastLED.h>
 
#define LED_PIN     2
#define NUM_LEDS    12
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB


CRGB leds[NUM_LEDS];                  // Array zur Speicherung des LED-Zustands
int fastled_delay = 25;               // eine weitere Möglichkeit, die Geschwindigkeit zu regulieren, falls zu schnell
int animSpeed = 1;                    // NICHT in s oder ms: grössere Zahl -> schneller

// mode identifier
String anim_mode = "all_equally";     // fade_to_color, running_lights, all_equally, pulse
 

// mode: running_lights, all_equally | Deklaration einer externen Farbpalette (rot-weiß-blau) -> extern: woanders im Code, progmem: gespeichert im flash, nicht im RAM.
extern const TProgmemPalette16 palette_redblue FL_PROGMEM;
extern const TProgmemPalette16 palette_blackwhite FL_PROGMEM;
CRGBPalette16 currentPalette;         // hier wird die aktuelle Farbpalette gespeichert.
TBlendType    currentBlending;        // LINEARBLEND, NOBLEND




// mode: pulse
int pulse_duration = 8000;            // in ms
CRGB pulse_color1 = CRGB(255, 0, 0);
CRGB pulse_color2 = CRGB(255, 255, 0);

// mode: fade_to_color
float target_color_r, target_color_g, target_color_b;

// mode: running_lights
static uint8_t colorIndex = 0;        // Laufindex, der angibt, welche Farbe aus der Palette aktuell verwendet wird. 0-255. Er wird kontinuierlich hochgezählt, 0: 1. Farbe/ 255: letzte Farbe in der Palette
float spot_narrowness = 8.0;          // 8 ist Standardwert. Je kleiner der Wert, desto breiter das Lauflicht


 
void setup() {
  delay( 3000 );                      // Sicherheitsverzögerung beim Start
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentBlending = LINEARBLEND;
  FastLED.clear ();
  config_anim_pulse();

  Serial.begin(115200);
}
 
 

 

/******************************************************************************
 *  2. Config anims // called in loop
 ******************************************************************************/

// triggered with key 1 in serial monitor
void config_anim_pulse(){             // ohne Farbpalette
  anim_mode = "pulse"; 
  pulse_duration = 8000;                // ms
  pulse_color1 = CRGB(255, 0, 0);  // Rot
  pulse_color2 = CRGB(0, 0, 255);  // Blau
}


void config_anim_allequally_2(){      // mit Farbpalette
  currentPalette = palette_redblue;
  anim_mode = "all_equally";          // fade_to_color, running_lights, all_equally
  //colorIndex = 0;
  animSpeed = .05;                    // grössere Zahl -> schneller
  fastled_delay = 150;                // grössere Zahl -> langsamer
}


// triggered with key 2 in serial monitor
void config_anim_runninglights_1(){
  currentPalette = palette_blackwhite; // Farben wählen
  anim_mode = "running_lights";        // fade_to_color, running_lights, all_equally
  //colorIndex = 0;
  animSpeed = 1;                       // grössere Zahl -> schneller
  fastled_delay = 25;                  // grössere Zahl -> langsamer
  spot_narrowness = 21.0;              // grössere Zahl -> schmalere Anim
}

// triggered with key 3 in serial monitor
void config_anim_fadetocolor_1(){
  anim_mode = "fade_to_color";         // fade_to_color, running_lights, all_equally
  colorIndex = 0;
  target_color_r = 1.0;                // [0...1]
  target_color_g = 0.0;
  target_color_b = 1.0;
  animSpeed = 18;                      // höhere Zahl -> schneller
  fastled_delay = 25;                  // grössere Zahl -> langsamer
}

// triggered with key 4 in serial monitor
void config_anim_fadetocolor_2(){
  anim_mode = "fade_to_color";         // fade_to_color, running_lights, all_equally
  colorIndex = 0;
  target_color_r = 1.0;                // [0...1]
  target_color_g = 1.0;
  target_color_b = 0.0;
  animSpeed = 8;                       // höhere Zahl -> schneller
  fastled_delay = 25;                  // grössere Zahl -> langsamer
}



/******************************************************************************
 *  3. write anims // called in loop, after configuring (see above)
 ******************************************************************************/



void anim_lights(){
  colorIndex = colorIndex + 1;           // höhere Zahl -> schneller



  if(anim_mode == "pulse"){
    uint32_t time = millis() % pulse_duration;
    float t = (float)time / pulse_duration;  
    float blendFactor = 0.5 * (1 - cos(t * 2 * PI)); // Volle Sinuswelle für Hin- und Rückblendung

    CRGB blendedColor = CRGB(
        (1 - blendFactor) * pulse_color1.r + blendFactor * pulse_color2.r,
        (1 - blendFactor) * pulse_color1.g + blendFactor * pulse_color2.g,
        (1 - blendFactor) * pulse_color1.b + blendFactor * pulse_color2.b
    );
    fill_solid(leds, NUM_LEDS, blendedColor);
  }
  
  
  if(anim_mode == "fade_to_color"){
    CRGB target_color( target_color_r * BRIGHTNESS, target_color_g * BRIGHTNESS, target_color_b * BRIGHTNESS);
    // fade all existing pixels toward target_color by "1" (out of 255) -> 1 is slow, 255 is fast
    for( uint16_t i = 0; i < NUM_LEDS; i++) {
      nblendU8TowardU8( leds[i].red,   target_color.red,   animSpeed);
      nblendU8TowardU8( leds[i].green, target_color.green, animSpeed);
      nblendU8TowardU8( leds[i].blue,  target_color.blue,  animSpeed);
    }
  }


  else if (anim_mode == "running_lights") {
    colorIndex += animSpeed;
    for (int i = 0; i < NUM_LEDS; ++i) {
      leds[i] = ColorFromPalette(currentPalette, colorIndex + (i * spot_narrowness), BRIGHTNESS, currentBlending);
    }
  }


  else if(anim_mode == "all_equally"){
    colorIndex += animSpeed;            // Geschwindigkeit anpassen
    CRGB color = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    for (int i = 0; i < NUM_LEDS; ++i) {
      leds[i] = color;                  // Alle LEDs bekommen die gleiche Farbe
    }
  }


  FastLED.show();
  FastLED.delay(fastled_delay);
  /*
  EVERY_N_MILLISECONDS(10) {            // statt FastLED.delay()
    FastLED.show();
  }
  */
}


// needed by fade_to_color: Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& current_color, const uint8_t target_color, uint8_t animSpeed){
  if( current_color == target_color) return;
  
  if( current_color < target_color ) {
    uint8_t delta = target_color - current_color;
    delta = scale8_video( delta, animSpeed);
    current_color += delta;
  } else {
    uint8_t delta = current_color - target_color;
    delta = scale8_video( delta, animSpeed);
    current_color -= delta;
  }
}


/******************************************************************************
 *  1. Start: trigger anims
 ******************************************************************************/



void loop(){
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();

    if (incomingChar == '1') {
      Serial.println("eins");
      config_anim_pulse();
      
    } else if (incomingChar == '2') {
      Serial.println("zwei");
      config_anim_runninglights_1();
    } 
    else if (incomingChar == '3') {
      Serial.println("drei");
      config_anim_fadetocolor_1();
    } 
    else if (incomingChar == '4') {
      Serial.println("vier");
      config_anim_fadetocolor_2();
    }
  }

  anim_lights();                  // muss im Loop stehen
}


 
// This example shows how to set up a static color palette which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this takes up 64 bytes of flash.
// Es müssen wirklich alle 16 Werte angegeben werden, sonst sind die restlichen schwarz
const TProgmemPalette16 palette_redblue FL_PROGMEM = {
    CRGB::Aqua,   CRGB::Blue,   CRGB::Aqua,   CRGB::Blue,
    CRGB::Aqua,   CRGB::Blue,   CRGB::Aqua,   CRGB::Blue,
    CRGB::Aqua,   CRGB::Blue,   CRGB::Aqua,   CRGB::Blue,
    CRGB::Aqua,   CRGB::Blue,   CRGB::Aqua,   CRGB::Blue
};

const TProgmemPalette16 palette_blackwhite FL_PROGMEM = {
    CRGB::Black,  CRGB::Black,  CRGB::Black,  CRGB::Black,
    CRGB::Black,  CRGB::Black,  CRGB::Black,  CRGB::Black,
    CRGB::Aqua,   CRGB::Aqua,   CRGB::Aqua,   CRGB::Aqua,
    CRGB::Aqua,   CRGB::Aqua,   CRGB::Aqua,   CRGB::Aqua    
};
 
