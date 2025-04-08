/*********************************************************
 *  15_Keypad.ino
 *  read matrix membrane keypads
 *  Install library "keypad" by Mark Stanley
 *  Die Anschlüsse am Keypad von links nach rechts: 
 *  Zunächst die 4 Reihen, dann die 3 Spalten
 *  Verbinde die 7 Pins des Keypads von links nach rechts mit 
 *  folgenden Pins des ESP32-C6: 8, 1, 0, 7, 6, 5, 4
 *  GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/09_Sensoren_testen/15_Keypad/15_Keypad.ino
 *********************************************************/


#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3; 

const int row1pin = 8; 
const int row2pin = 1; 
const int row3pin = 0; 
const int row4pin = 7; 
const int column1pin = 6; 
const int column2pin = 5; 
const int column3pin = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {row1pin, row2pin, row3pin, row4pin};   // Pins connected to mc (left to 4th)
byte colPins[COLS] = {column1pin, column2pin, column3pin};   // Pins connected to mc (5th to right)

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(115200);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}