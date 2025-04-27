/******************************************************************************
 *  Bodenfeuchtigkeitssensor_Wasserpumpe.ino
 *  Drehe ewig in eine Richtung
 *  Schöne Anleitung: https://docs.sunfounder.com/projects/3in1-kit-v2/de/latest/components/component_l9110_module.html#cpn-l9110
 *  Der DC-Motor (z. B. Kreiselpumpe) ist z. B. über den Stepper-Driver L9110  mit dem ESP32-C6 verbunden
 *  Verbinde IN1-4 am Stepper-Driver mit GPIO18-21 am ESP32-C6
 *  Driver: A_1A          <->  ESP32-C6: 10
 *  Driver: A_1B          <->  ESP32-C6: 11
 *  Driver: Vcc           <->  esterne Stromversorgung (2.5V - 12V)
 *  Driver: GND           <->  GND an ESP32-C6 und der esternen Stromversorgung
 *  Info: Neben A-1A und A-1B hat der Stepper-Driver L9110 zudem die Anschlüsse B-1A und B-1B, die für einen zweiten Motor gedacht sind.
 *  GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/10_Aktoren_testen/09_DC-Motor_Pumpe/09_DC-Motor_Pumpe.ino
 *******************************************************************************/


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