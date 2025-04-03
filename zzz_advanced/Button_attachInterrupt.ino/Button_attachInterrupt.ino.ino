/******************************************************************************************************
 * Button_attachInterrupt.ino
 * receive eg.  button / reed sensor / PIR sensor / touch sensor (TTB223B)/ light sensor (LM393) input 
 * and print value to serial port
 * Sensor: Vin  <->  ESP32-C6: 3.3V 
 * Sensor: GND  <->  ESP32-C6: GND
 * Sensor: Data <->  ESP32-C6: GPIO7
 ******************************************************************************************************/


const byte interruptPin = 7;
volatile byte state = 0;
int count = 0;

void setup() {
  pinMode(interruptPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(interruptPin), myTask, CHANGE);
  Serial.begin(115200);
}

void loop() {
  count++;
  Serial.println(count);
  delay(1000);
}

void myTask() {
  if(state == 0){
    Serial.println("button pressed");
    state = 1;
  }
  else if(state == 1){
    Serial.println("button released");
    state = 0;
  }
}
