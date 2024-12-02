// SEND BUTTON DATA FROM ESP32 TO SERIALPORT

const int buttonPin = 7;
const int led = BUILTIN_LED;      // LED pin number (use pin 13 for builtin LED)

int buttonState = 0;         
int prev_buttonState = 0;

void setup() {  
  Serial.begin(115200);  // match default TouchDesigner Serial In DAT baudrate.
  pinMode(buttonPin, INPUT);   
  pinMode(led, OUTPUT);   
  digitalWrite(led, 0);
}

void loop(){
  buttonState = digitalRead(buttonPin);
  if(buttonState == prev_buttonState) {
    return;
  }

  prev_buttonState = buttonState;
 
  if (buttonState == 1) {
    digitalWrite(led, 1);
    Serial.println(1);
  } else {
    digitalWrite(led, 0);
    Serial.println(0);
  } 
}