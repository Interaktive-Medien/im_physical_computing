/************************************************************************
 * Gassensor.ino
 * receive analog input from MQ-135 and print value to serial port
 * Sensor: AO oder DO  <->  ESP32-C6: 4 
 * Sensor: VCC  <->  ESP32-C6: 5V 
 * Sensor: GND  <->  ESP32-C6: GND
 * Für bessere Ergebnisse und mehr Langlebigkeit des Microcontrollers 
 * sollte das Datensignal des Gas-Sensors (max. 5V) mit einem Level Shifter
 * oder einem Spannungsteiler auf den Wertebereich 0...3.3V heruntergeregelt werden.   
 ************************************************************************/




int sensorValue;
int sensorpin = 4;

void setup(){  
  Serial.begin(115200);                      //   sets the serial port to 9600
 }

void loop(){
  sensorValue = analogRead(sensorpin);       //   read analog input pin 0
  Serial.println(sensorValue, DEC);          //   prints the value read (PPM)
  delay(500);
}