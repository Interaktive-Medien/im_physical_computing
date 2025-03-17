/***********************************************************************
 * RotaryEncoder_Button.ino
 * receive rotary encoder input and button input, print values to serial port
 * Seite am Rotary Encoder mit 2 Anschlüssen (Button): 
 * Sensor: 1. von links  <->  ESP32-C6: 3.3V 
 * Sensor: 2. von links  <->  ESP32-C6: GPIO 7
 * Seite am Rotary Encoder mit 3 Anschlüssen (Rotary Encoder): 
 * Sensor: 1. von links (A)  <->  ESP32-C6: GPIO 2
 * Sensor: 2. von links (GND)  <->  ESP32-C6: GND
 * Sensor: 3. von links (B)  <->  ESP32-C6: GPIO 3
 ***********************************************************************/

// Define rotary encoder pins
#define ENC_A 2 
#define ENC_B 3

// Define button pin
const int buttonPin = 7;
const int ledPin = BUILTIN_LED;

// rotary encoder
unsigned long _lastIncReadTime = micros(); 
unsigned long _lastDecReadTime = micros(); 
int _pauseLength = 25000;
int _fastIncrement = 10;

volatile int counter = 0;
int buttonState = 0;
int prev_buttonState = 0;

void setup() {
  // Set encoder pins and attach interrupts
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  // Set button pin
  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);

  // Start the serial monitor to show output
  Serial.begin(115200);
}

void loop() {
  static int lastCounter = 0;

  // If count has changed print the new value to serial
  if (counter != lastCounter) {
    Serial.println(counter);
    lastCounter = counter;
  }

  // Read button state
  buttonState = digitalRead(buttonPin);
  if (buttonState != prev_buttonState) {
    prev_buttonState = buttonState;
    if (buttonState == 1) {
      digitalWrite(ledPin, 1);   // turn LED on
      Serial.println("Button Pressed");
    } else {
      digitalWrite(ledPin, 0);   // turn LED off
      Serial.println("Button Released");
    }
  }
}

void read_encoder() {
  static uint8_t old_AB = 3;  // Lookup table index
  static int8_t encval = 0;   // Encoder value  
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  old_AB <<=2;  // Remember previous state

  if (digitalRead(ENC_A)) old_AB |= 0x02; // Add current state of pin A
  if (digitalRead(ENC_B)) old_AB |= 0x01; // Add current state of pin B
  
  encval += enc_states[( old_AB & 0x0f )];

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if (encval > 3) {        // Four steps forward
    int changevalue = 1;
    if ((micros() - _lastIncReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastIncReadTime = micros();
    counter += changevalue;
    encval = 0;
  } else if (encval < -3) { // Four steps backward
    int changevalue = -1;
    if ((micros() - _lastDecReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastDecReadTime = micros();
    counter += changevalue;
    encval = 0;
  }
}
