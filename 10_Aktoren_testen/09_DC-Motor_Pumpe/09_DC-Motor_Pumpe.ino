/******************************************************************************
 *  09_DC-Motor.ino
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

const int motor_in1 = 11;
const int motor_in2 = 10;

void setup()
{
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  
  digitalWrite(motor_in1, 1);     // HIGH
  digitalWrite(motor_in2, 0);     // LOW
}

void loop()
{

}
