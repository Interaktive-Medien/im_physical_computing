/******************************************************************************************************
 " Kap. 7: Serielle Kommunikation"
 * 07_Serielle_Kommunikation.ino
 * receive button input and print value to serial port
 " Connect ...
 * Sensor: Vin  <->  ESP32-C6: 3.3V 
 * Sensor: GND  <->  ESP32-C6: GND  (nicht bei Button, Reedachalter)
 * Sensor: Data <->  ESP32-C6: GPIO 7
  ******************************************************************************************************/

const int buttonPin = 7;
const int ledPin = BUILTIN_LED;
int buttonState = 0;

void setup(){
    Serial.begin(115200);
    pinMode(buttonPin, INPUT_PULLDOWN);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, 0); // turn LED off:
}

void loop(){
    buttonState = digitalRead(buttonPin);

    if (buttonState == 1){
        digitalWrite(ledPin, 1);      // turn LED on:
        Serial.println(1);
    }
    else{
        digitalWrite(ledPin, 0);      // turn LED off:
        Serial.println(0);
    }
}
