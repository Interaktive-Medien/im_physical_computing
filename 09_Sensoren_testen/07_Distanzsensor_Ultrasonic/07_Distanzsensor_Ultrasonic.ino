/******************************************************************************
 * 07_Distanzsensor_Ultrasonic.ino
 * receive ultrasonic sensor input (HC-SR04) and print value to serial port
 * Installiere Adafruit_VL6180X Library von Adafruit.
 * 4 Anschlüsse an ESP32-C6 (Kommunikation per I2C -> dafür braucht es die vorinstallierte Wire-Library):
 * Sensor: Vin  <->  ESP32-C6: 3.3V 
 * Sensor: GND  <->  ESP32-C6: GND
 * Sensor: Trig <->  ESP32-C6: GPIO12
 * Sensor: Echo <->  ESP32-C6: GPIO13
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/09_Sensoren_testen/07_Distanzsensor_Ultrasonic/07_Distanzsensor_Ultrasonic.ino
 ******************************************************************************/

const int trigPin = 12;
const int echoPin = 13;

#define SOUND_SPEED 0.034                  // define sound speed in cm/uS

long duration;
float distanceCm;

void setup() {
  Serial.begin(115200);                    // Starts the serial communication
  pinMode(trigPin, OUTPUT);                // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);                 // Sets the echoPin as an Input
}

void loop() {
  digitalWrite(trigPin, LOW);              // Clears the trigPin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);             // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);       // Reads the echoPin, returns the sound wave travel time in microseconds
  distanceCm = duration * SOUND_SPEED/2;   // Calculate the distance

  Serial.print("Distance (cm): ");         // Prints the distance in the Serial Monitor
  Serial.println(distanceCm);
  
  delay(1000);
}