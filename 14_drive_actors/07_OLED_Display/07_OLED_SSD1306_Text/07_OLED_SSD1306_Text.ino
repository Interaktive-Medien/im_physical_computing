/************************************************************
 * 07_OLED_SSD1306_Text.ino
 * Zeige Text auf SSD1306 display
 * installiere Arduino Library Adafruit SSD1306. 
 * Dependency libraries Adafruit BusIO and Adafruit GFX Library 
 * will be installed automatically as well.
 * Connect: 
 * ESP32-C6: 3.3V   <->  SSD1306: VDD
 * ESP32-C6: GND    <->  SSD1306: GND
 * ESP32-C6: GPIO6  <->  SSD1306: SDA
 * ESP32-C6: GPIO7  <->  SSD1306: SCK
 ************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// I2C Pins für ESP32-C6
#define I2C_SDA 6
#define I2C_SCL 7
#define i2c_Address 0x3c      // initialisiere mit Standard I2C addr 0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // -1 = kein Reset-Pin

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);              // I2C mit eigenen Pins initialisieren

    // Display initialisieren
    if (!display.begin(i2c_Address, true)) {  
        Serial.println("SSD1306 nicht gefunden!");
    }
    
    Serial.println("Display erfolgreich initialisiert!");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.println("Bun di");
    display.display();
}

void loop() {
}
