/*****************************************************
 * 06_LCD_display.ino
 * Show 16*2 digits text on display
 * Installiere Library "LiquidCrystal I2C" by Frank Brabander
 * I2C LCD1602 Modul (module needs 5V !!)
 * Anschluss:
 * ESP32-C6: GND    <->  I2C LCD1602: GND
 * ESP32-C6: 5V     <->  I2C LCD1602: VCC
 * ESP32-C6: GPIO6  <->  I2C LCD1602: SDA
 * ESP32-C6: GPIO7  <->  I2C LCD1602: SCL
 *****************************************************/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD-Initialisierung: Adresse 0x27, 16 Spalten, 2 Zeilen
// Adresse mit dem I2C Scanner herausfinden: Examples>Wire>WireScan.ino
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup()
{
  // I2C-Pins festlegen: SDA = GPIO6, SCL = GPIO7
  Wire.begin(6, 7);

  // LCD initialisieren
  lcd.init();  
  lcd.backlight();  
}

void loop()
{
  lcd.setCursor(2, 0);      // Cursor auf Spalte 2, Zeile 0 setzen
  lcd.print("kein-bock.ch"); 
  
  lcd.setCursor(0, 1);      // Cursor auf Spalte 0, Zeile 1 setzen
  lcd.print("huhu"); 
  
  //scroll
  /*
  for(int i = 0; i < 45; i++){
    lcd.scrollDisplayLeft();
    delay(500);
  }
  */
  
  // extra character
  /* 
  lcd.write(126);  // Pfeil siehe https://www.sparkfun.com/datasheets/LCD/HD44780.pdf Code A00 (S.17)
  */
}
