/**************************************************************************************
 *  SavePersistentVal.ino
 *  change persistentVal, also change data type to int, String, float, ...
 *  Older manuals use EEPROM.write(); and EEPROM.read(); They can't hold values > 255
 **************************************************************************************/

#include <EEPROM.h>
String persistentVal = "capricorn";

void setup() {
  Serial.begin(115200);          //  Serielle Kommunikation starten
  delay(100);                    //  Kurze Wartezeit für Stabilität
  
  EEPROM.begin(128);             //  Speiecher initialisieren (128 Bytes reservieren - ein int braucht 4 Byte, ein Buchstebe 1 Byte)

  EEPROM.put(0, persistentVal);  //  Wert speichern in der ersten Seicherstelle (0)
  EEPROM.commit();               //  Speicherung bestätigen
  Serial.printf("Saved %d persistently \n", persistentVal);
}

void loop() {
}
