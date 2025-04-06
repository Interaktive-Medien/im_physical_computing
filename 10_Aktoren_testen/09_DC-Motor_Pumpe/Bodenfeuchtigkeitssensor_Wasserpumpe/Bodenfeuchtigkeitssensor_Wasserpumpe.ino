/************************************************************************
 *  Bodenfeuchtigkeitssensor_Wasserpumpe.ino
 *  Lese den Feuchtigkeitssensor aus und schreibe sie an den seriellen Port.
 *  Wenn es zu trocken ist, pumpe Wasser in die Erde
 *  Feuchtigkeitssensor:
 *  Sensor: +      <->  ESP32-C6: 3.3V 
 *  Sensor: -      <->  ESP32-C6: GND
 *  Sensor: AOUT   <->  ESP32-C6: GPIO4
 *  Wasserpumpe:
 *  Driver: A_1A   <->  ESP32-C6: 10
 *  Driver: A_1B   <->  ESP32-C6: 11
 *  Driver: Vcc    <->  esterne Stromversorgung (2.5V - 12V)
 *  Driver: GND    <->  GND an ESP32-C6 und der esternen Stromversorgung
 ************************************************************************/

/////////////////////////////////// Feuchtigkeitssensor
const int sensorPin = 4;
int sensorValue = 0;

/////////////////////////////////// Wasserpumpe
const int motor_in1 = 11;
const int motor_in2 = 10;

void setup() {
  Serial.begin(115200);
  delay(1000);

  /////////////////////////////////// Wasserpumpe
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
}

void loop() {
  /////////////////////////////////// Wasserpumpe
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  if(sensorValue > 1500){            // je grösser der Wert, desto trockener
    Serial.println("Erde ist sehr trocken und muss bewässert werden.");
    /////////////////////////////////// Wasserpumpe
    digitalWrite(motor_in1, 1);      // pumpen
  }
  else{
    Serial.println("Erde ist feucht genug.");
    /////////////////////////////////// Wasserpumpe
    digitalWrite(motor_in1, 0);      // stop
  }
  delay(2000);
}