/*********************************************
 * JSON_decode_example.ino
 * Turn JSON string into an object
 * Install library "Arduino_JSON" by Arduino
 *********************************************/

#include <Arduino_JSON.h>

const char input[] = "{\"result\":true,\"count\":42,\"foo\":\"bar\"}";

void setup() {
  Serial.begin(115200);
  while (!Serial);
  demoParse();
}

void loop() {
}

void demoParse() {
  JSONVar myObject = JSON.parse(input);
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  if (myObject.hasOwnProperty("count")) {
    Serial.print("count = ");
    Serial.println(myObject["count"]);
  }
}