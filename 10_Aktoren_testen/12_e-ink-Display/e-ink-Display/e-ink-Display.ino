// #include <GxEPD2_3C.h> // Library für 3-Farben Displays
#include <GxEPD2_4C.h>
#include <Fonts/FreeSansBold9pt7b.h> // Eine schöne Standardschrift
// Display Library selection
#include "GxEPD2_display_selection_new_style.h"

// --- DISPLAY DEFINITION (Der Teil, der gefehlt hat) ---
// Für das 4.2" 3-Farben Modul (G/Gelb oder R/Rot) an einem ESP32:
// Pins: CS=5, DC=17, RST=16, BUSY=4
// GxEPD2_3C<GxEPD2_420c_Z21, GxEPD2_420c_Z21::HEIGHT> display(GxEPD2_420c_Z21(/*CS=*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

// Versuch 2: Falls Versuch 1 nicht geht (älterer Chip)
// GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(5, 17, 16, 4));

// Versuch 3: Speziell für einige neuere Revisionen
// GxEPD2_3C<GxEPD2_420c_Z08, GxEPD2_420c_Z08::HEIGHT> display(GxEPD2_420c_Z08(5, 17, 16, 4));



void setup() {
  // Initialisierung
  // display.init(115200);

  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Raumtracker Display Start (4C) ===");
  display.init(115200, true, 2, false);


  
  // Der "Page-Loop" - Wichtig für E-Paper (spart RAM)
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE); // Hintergrund weiß
    
    // 1. Text in Schwarz
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(20, 50);
    display.print("Hallo Welt!");

    // 2. Text in Gelb
    display.setTextColor(GxEPD_YELLOW);
    display.setCursor(20, 100);
    display.print("Das ist Gelb!");
    
    // 3. Eine einfache Linie zur Deko
    display.drawLine(20, 110, 200, 110, GxEPD_BLACK);

  } while (display.nextPage());
  
  // Schlafen legen, um das Display zu schonen
  display.hibernate(); 
}

void loop() {
  // E-Paper brauchen keinen Code im Loop, da das Bild statisch bleibt
}