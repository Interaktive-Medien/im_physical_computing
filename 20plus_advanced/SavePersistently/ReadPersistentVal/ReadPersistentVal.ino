/**************************************************************************************
 *  ReadPersistentVal.ino
 *  change data type of persistentVal -> int, String, float, ...
 *  Older manuals use EEPROM.write(); and EEPROM.read(); They can't hold values > 255
 **************************************************************************************/


#include <EEPROM.h>

String persistentVal;

void setup() {
  Serial.begin(115200);                //  Serielle Kommunikation starten
  delay(100);                          //  Kurze Wartezeit für Stabilität

  EEPROM.begin(128);                   //  Speicher initialisieren (128 Bytes reservieren - ein int braucht 4 Byte, ein Buchstebe 1 Byte)
  EEPROM.get(0, persistentVal);        //  Letzten gespeicherten Wert auslesen und ausgeben
}

void loop() {
  Serial.println(persistentVal);
  delay(3000);
}
