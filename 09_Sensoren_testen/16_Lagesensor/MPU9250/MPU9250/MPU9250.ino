/**************
 * X- und Y- Drehung mit Kalman Filter
 * Installiere MPU9250_asukiaaa by Asuki Kono
 ***************************************/


#include <Wire.h>
#include <MPU9250_asukiaaa.h>
#include <math.h>

constexpr int SDA_PIN = 6;
constexpr int SCL_PIN = 7;

MPU9250_asukiaaa mpu;

float pitch = 0.0, roll = 0.0;
float gyroPitch = 0.0, gyroRoll = 0.0;

float kalmanPitch = 0.0, kalmanRoll = 0.0;
float kalmanPitchBias = 0.0, kalmanRollBias = 0.0;
float pitchError = 0.0, rollError = 0.0;

unsigned long lastTime;

const float alpha = 0.98; // Gewichtung für Kalman-like Fusion

void setup() {
  Serial.begin(115200);
  delay(2000);

  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.setWire(&Wire);

  mpu.beginAccel();
  mpu.beginGyro();

  // Initialisieren mit aktuellen Werten
  mpu.accelUpdate();
  mpu.gyroUpdate();

  pitch = atan2(mpu.accelY(), sqrt(pow(mpu.accelX(), 2) + pow(mpu.accelZ(), 2))) * 180 / PI;
  roll  = atan2(-mpu.accelX(), mpu.accelZ()) * 180 / PI;

  kalmanPitch = pitch;
  kalmanRoll = roll;

  lastTime = millis();
}

void loop() {
  mpu.accelUpdate();
  mpu.gyroUpdate();

  // Zeit seit letztem Update
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // --- Accelerometer: Neigung berechnen (in Grad)
  float accPitch = atan2(mpu.accelY(), sqrt(pow(mpu.accelX(), 2) + pow(mpu.accelZ(), 2))) * 180 / PI;
  float accRoll  = atan2(-mpu.accelX(), mpu.accelZ()) * 180 / PI;

  // --- Gyroskop: Integration der Winkelgeschwindigkeit
  float gyroXrate = mpu.gyroX(); // Roll-Rate
  float gyroYrate = mpu.gyroY(); // Pitch-Rate

  gyroPitch += gyroYrate * dt;
  gyroRoll  += gyroXrate * dt;

  // --- Komplementärfilter (Kalman-like)
  kalmanPitch = alpha * (kalmanPitch + gyroYrate * dt) + (1 - alpha) * accPitch;
  kalmanRoll  = alpha * (kalmanRoll  + gyroXrate * dt) + (1 - alpha) * accRoll;

  // --- Ausgabe
  Serial.print("Pitch (Y): "); Serial.print(kalmanPitch, 2); Serial.print("°\t");
  Serial.print("Roll (X): ");  Serial.print(kalmanRoll, 2);  Serial.println("°");

  delay(50);
}
