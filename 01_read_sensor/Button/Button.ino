const int buttonPin = 7;
const int ledPin =  BUILTIN_LED; 
int buttonState = 0;         
int prev_buttonState = 0;

void setup() {  
    Serial.begin(115200); 
    pinMode(buttonPin, INPUT);  // initialize the pushbutton pin as an input:
    pinMode(ledPin, OUTPUT);    // initialize the LED pin as output:
    digitalWrite(ledPin, 0);    // turn LED off:
}

void loop(){
    buttonState = digitalRead(buttonPin);
    if(buttonState == prev_buttonState) {
        return;
    }
    prev_buttonState = buttonState;
    if (buttonState == 1) {
        digitalWrite(ledPin, 1);   // turn LED on:
        Serial.println(1);
    } else {
        digitalWrite(ledPin, 0);  // turn LED off:
        Serial.println(0);
    } 
}
