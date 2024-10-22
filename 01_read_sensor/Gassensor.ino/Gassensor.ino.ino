/************************************************************************
 * Gassensor.ino
 * receive analog input from MQ-135 and print value to serial port
 ************************************************************************/


int sensorValue;
int sensorpin = 0;

void   setup(){  
  Serial.begin(115200);                            //   sets the serial port to 9600
 }

void loop(){
  sensorValue = analogRead(sensorpin);       //   read analog input pin 0
  Serial.print("AirQua=");
  Serial.print(sensorValue,   DEC);               // prints the value read
  Serial.println(" PPM");                                 //   wait 100ms for next reading
}