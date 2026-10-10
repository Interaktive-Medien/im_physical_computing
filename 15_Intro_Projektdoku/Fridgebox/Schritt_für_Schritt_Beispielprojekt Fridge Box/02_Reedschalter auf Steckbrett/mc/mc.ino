/******************************************************************************************************
 * Fridge Box
 * Schritt 2: Empfange Daten des Reed-Sensors und gebe sie auf dem seriellen Monitor + LED aus

 ***************  Schritt 2: Reed-Schalter
 * Reed-Sensor: eine Seite     <->  3.3V  
 * Reed-Sensor: andere Seite   <->  ESP32-C6: GPIO 10 (input)
  ******************************************************************************************************/

const int sensorPin = 10;
const int ledPin =  BUILTIN_LED; 
int sensorState = 0;         
int prev_sensorState = 0;

void setup() {  
    Serial.begin(115200); 
    pinMode(sensorPin, INPUT_PULLDOWN);  // initialize the pushbutton pin as an input:
    pinMode(ledPin, OUTPUT);             // initialize the LED pin as output:
    digitalWrite(ledPin, 0);             // turn LED off:
}

void loop(){
    sensorState = digitalRead(sensorPin);
    if(sensorState == prev_sensorState) {
        return;
    }
    prev_sensorState = sensorState;
    if (sensorState == 1) {
        digitalWrite(ledPin, 1);          // turn LED on:
        Serial.println(1);
    } else {
        digitalWrite(ledPin, 0);          // turn LED off:
        Serial.println(0);
    } 
}
