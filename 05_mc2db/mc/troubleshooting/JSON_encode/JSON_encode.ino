/*********************************************
 * JSON_encode.ino
 * Turn  object into JSON-formatted string (stringify)
 * Install library "Arduino_JSON" by Arduino
 *********************************************/


#include <Arduino_JSON.h> // by Arduino

void setup() {
  Serial.begin(115200);

  JSONVar myObject;

  myObject["string"] = "world";
  myObject["boolean"] = true;
  myObject["int"] = (int) 42;
  myObject["long"] = (long) 42;
  myObject["unsigned long"] = (unsigned long) 42;
  myObject["float"] = (float) 42.1;


  // JSON.stringify(myVar) can be used to convert the JSONVar to a String
  String jsonString = JSON.stringify(myObject);

  Serial.print("JSON.stringify(myObject) = ");
  Serial.println(jsonString);
}

void loop() {}
