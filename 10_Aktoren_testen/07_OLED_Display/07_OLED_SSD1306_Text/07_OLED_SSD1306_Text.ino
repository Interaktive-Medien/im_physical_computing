/************************************************************
 * 07_OLED_SSD1306_Text.ino
 * Zeige Text auf SSD1306 display
 * installiere Arduino Library Adafruit SSD1306. 
 * Dependency libraries Adafruit BusIO and Adafruit GFX Library 
 * will be installed automatically as well.
 * Connect: 
 * ESP32-C6: 3.3V / 5V   <->  SSD1306: VDD
 * ESP32-C6: GND         <->  SSD1306: GND
 * ESP32-C6: GPIO6       <->  SSD1306: SDA
 * ESP32-C6: GPIO7       <->  SSD1306: SCK
 ************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// I2C Pins für ESP32-C6
#define I2C_SDA 6
#define I2C_SCL 7   
#define I2C_ADDRESS   0x3C                   // initialisiere mit Standard I2C addr 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);              // I2C mit eigenen Pins initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20, 20);
  display.println("Bun di !!");
  display.display();
}

void loop() {}