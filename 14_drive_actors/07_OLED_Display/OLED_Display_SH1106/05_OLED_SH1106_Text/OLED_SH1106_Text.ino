/************************************************************
 * show content on SH1106 display
 * install Arduino library Adafruit SH1106. 
 * Dependency libraries Adafruit BusIO and Adafruit GFX Library will be installed automatically as well.
 * Connect: 
 * ESP32-C6: 3.3V   <->  SH1106: VDD
 * ESP32-C6: GND    <->  SH1106: GND
 * ESP32-C6: GPIO5  <->  SH1106: SDA
 * ESP32-C6: GPIO5  <->  SH1106: SCK
 ************************************************************/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// I2C Pins für ESP32-C6
#define I2C_SDA 5
#define I2C_SCL 6
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

// Display-Definition (128x64 oder 128x32, je nach Modell)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // -1 = kein Reset-Pin

void setup() {
    Serial.begin(115200);
    
    // I2C mit eigenen Pins initialisieren
    Wire.begin(I2C_SDA, I2C_SCL);

    // Display initialisieren
    if (!display.begin(i2c_Address, true)) {  // 0x3C ist die Standardadresse für SH1106
        Serial.println("SH1106 nicht gefunden!");
    }
    
    Serial.println("Display erfolgreich initialisiert!");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(10, 10);
    display.println("Bun di");
    display.display();
}

void loop() {
 
}
