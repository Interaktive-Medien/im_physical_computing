/*******************************************************************************
 * Sende Button-Signal per serieller Kommunikation (zur Nutzung in z. B. TouchDesigner)
 * Empfange Signal per serieller Kommunikation von z. B. TouchDesigner.
 * Es gibt visuelles Feedback auf der RGB_BUILTIN led: Senden: gelb, empfangen: blau
 * Verbinde Button Input mit ESP32-C6 an GPIO 7
 ********************************************************************************/


// set pin numbers:

const int buttonPin = 7;
const int led =  RGB_BUILTIN;

int buttonState = 0;         
int prev_buttonState = 0;

void setup() 
{  
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLDOWN);  
  pinMode(led, OUTPUT);    
  digitalWrite(led, 0);
}

void loop()
{
  sendToTD(); // send button data to TouchDesigner
  receiveFromTD();
}


// send button data to TouchDesigner
void sendToTD(){
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == prev_buttonState) {
    return;
  }

  prev_buttonState = buttonState;

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == 1) {
    // turn LED on:
    Serial.println(1);
    rgbLedWrite(led, 255, 255, 0);  // gelb
  } else {
    // turn LED off:
    Serial.println(0);
    digitalWrite(led, 0);
  } 
}

void receiveFromTD(){
  // receive serial data from TouchDesigner
  
  // buttonState = digitalRead(buttonPin);

  if (Serial.available()){
    char c = Serial.read();
    Serial.println(c);  
    
    if (c == '1')
      rgbLedWrite(led, 0, 0, 255);  // blau
    else if (c == '0')
      digitalWrite(led, 0);  // turn off
  }
}
