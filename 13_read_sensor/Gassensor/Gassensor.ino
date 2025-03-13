/************************************************************************
 * Gassensor.ino
 * receive analog input from MQ-135 and print value to serial port
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