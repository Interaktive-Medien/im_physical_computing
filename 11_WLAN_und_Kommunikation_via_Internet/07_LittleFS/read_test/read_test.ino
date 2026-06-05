/***********************************************
 * Use of little FS
 * https://youtu.be/Qw5mQus-WwA?si=4I-cYm3dzz8KM40R
 * Read files from file system of ESP.
 * Upload your files (located at "/data") to the ES using CMD + SHIFT + P -> "Upload LittleFS to ESP..."
 * Use the Port "Family Device"
************************************************/

#include <Arduino.h>
#include "FS.h"
#include "LittleFS.h"

#define FORMAT_LITTLEFS_IF_FAILED true

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("go go");
  if(!LittleFS.begin()){
     Serial.println("An Error has occurred while mounting LittleFS");
     return;
  }
  delay(1000);
  File file = LittleFS.open("/text.txt", "r");
  if(!file){
     Serial.println("Failed to open file for reading");
     return;
  }
  //reading from a file and displaying it on the serial monitor
  Serial.println("File Content:");
  while(file.available()){
     Serial.write(file.read());
  }
  file.close();
}

void loop(){}