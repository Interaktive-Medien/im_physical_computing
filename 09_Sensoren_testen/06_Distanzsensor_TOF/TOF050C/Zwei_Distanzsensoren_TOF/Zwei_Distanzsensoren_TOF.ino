/**********************************************************************************************
*  Zwei_Distanzsensoren_TOF.ino
*  TOF050C Time-of-flight sensoren mit V16180x
*  Wenn sich ein Objekt mindestens einem von zwei Sensoren nähert (z. B. näher als 20mm), 
*. dann wird auf der seriellen Schnittstelle eine 1 ausgegeben. Sobald es sich entfernt wird eine 0 ausgegeben.
*  Installiere Adafruit_VL6180X Library von Adafruit.
*  Anschlüsse an ESP32-C6 (Kommunikation per I2C -> dafür braucht es die vorinstallierte Wire-Library):
*  Sensor 1 sowie 2: Vin  <->  ESP32-C6: 3.3V 
*  Sensor 1 sowie 2: GND  <->  ESP32-C6: GND
*  Sensor 1 sowie 2: SDA  <->  ESP32-C6: GPIO6
*  Sensor 1 sowie 2: SCL  <->  ESP32-C6: GPIO7
*  Sensor 1 XSHUT         <->  ESP32-C6: GPIO10
*  Sensor 2 XSHUT         <->  ESP32-C6: GPIO11
***********************************************************************************************/

#include <Wire.h>
#include <Adafruit_VL6180X.h>

#define I2C_SDA 6
#define I2C_SCL 7

#define XSHUT_1 10
#define XSHUT_2 11

#define TOF1_ADDRESS 0x30
#define TOF2_ADDRESS 0x31

// Getrennte Thresholds
#define THRESHOLD1 40  // Sensor 1
#define THRESHOLD2 40  // Sensor 2

Adafruit_VL6180X tof1 = Adafruit_VL6180X();
Adafruit_VL6180X tof2 = Adafruit_VL6180X();

bool objectNearby = false;
bool lastInRange = false;

void setup() {
  Serial.begin(115200);
  delay(100);
  Wire.begin(I2C_SDA, I2C_SCL);

  pinMode(XSHUT_1, OUTPUT);
  pinMode(XSHUT_2, OUTPUT);

    // Beide Sensoren deaktivieren
  digitalWrite(XSHUT_1, LOW);
  digitalWrite(XSHUT_2, LOW);
  delay(10);

  // Sensor 1 einschalten
  digitalWrite(XSHUT_1, HIGH);
  delay(10);
  if (!tof1.begin(&Wire)) {
    Serial.println(F("Sensor 1 konnte nicht gestartet werden"));
    while (1);
  }
  // Adresse direkt ändern, solange Sensor 2 noch ausgeschaltet ist
  tof1.setAddress(TOF1_ADDRESS);
  delay(10);  // Kurze Pause, um Adressänderung zu stabilisieren

  // Sensor 2 einschalten
  digitalWrite(XSHUT_2, HIGH);
  delay(10);
  if (!tof2.begin(&Wire)) {
    Serial.println(F("Sensor 2 konnte nicht gestartet werden"));
    while (1);
  }
  tof2.setAddress(TOF2_ADDRESS);
  delay(10);


  Serial.println("Beide Sensoren initialisiert.");
}

void loop() {
  uint8_t range1 = tof1.readRange();
  uint8_t status1 = tof1.readRangeStatus();

  uint8_t range2 = tof2.readRange();
  uint8_t status2 = tof2.readRangeStatus();

  bool inRange1 = (status1 == VL6180X_ERROR_NONE && (range1 <= THRESHOLD1 && range1 > 0));
  bool inRange2 = (status2 == VL6180X_ERROR_NONE && (range2 <= THRESHOLD2 && range2 > 0));

  Serial.print("Sensor 1: ");
  Serial.println(range1);
  Serial.print("Sensor 2: ");
  Serial.println(range2);

  bool currentInRange = inRange1 || inRange2;

  if (currentInRange != lastInRange) {
    delay(50);
    range1 = tof1.readRange();
    status1 = tof1.readRangeStatus();
    range2 = tof2.readRange();
    status2 = tof2.readRangeStatus();

    bool inRange1 = (status1 == VL6180X_ERROR_NONE && (range1 <= THRESHOLD1 && range1 > 0));
    bool inRange2 = (status2 == VL6180X_ERROR_NONE && (range2 <= THRESHOLD2 && range2 > 0));

    currentInRange = inRange1 || inRange2;

    if (currentInRange != objectNearby) {
      objectNearby = currentInRange;
      if (objectNearby) {
        Serial.println("1: Objekt in der Nähe");
      } else {
        Serial.println("0: Kein Objekt in der Nähe");
      }
      // Serial.print("Sensor 1: ");
      // Serial.println(range1);
      // Serial.print("Sensor 2: ");
      // Serial.println(range2);
    }
  }

  lastInRange = currentInRange;
  delay(1000);
}
