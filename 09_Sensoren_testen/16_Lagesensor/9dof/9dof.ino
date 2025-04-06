#include <Wire.h>
#include <Adafruit_ICM20948.h>

Adafruit_ICM20948 icm;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("🔍 Starte ICM20948 Initialisierung...");

    Wire.begin(6, 7);

    if (!icm.begin_I2C(0x68, &Wire)) {
        Serial.println("❌ ICM20948 nicht gefunden!");
        while (1);
    }
    Serial.println("✅ ICM20948 verbunden.");

    icm.setAccelRange(ICM20948_ACCEL_RANGE_4_G);
    icm.setGyroRange(ICM20948_GYRO_RANGE_500_DPS);
    Serial.println("✅ Setup abgeschlossen!");
}

void loop() {
    sensors_event_t accel, gyro, mag, temp;
    icm.getEvent(&accel, &gyro, &mag, &temp);

    // Berechnung Roll & Pitch aus Beschleunigungssensor
    float roll  = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
    float pitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI;

    // Berechnung Yaw aus Magnetometer
    float yaw = atan2(mag.magnetic.y, mag.magnetic.x) * 180.0 / PI;
    if (yaw < 0) yaw += 360;  

    // Ausgabe im gewünschten Format
    Serial.print("x:"); Serial.print((int)roll);
    Serial.print(", y:"); Serial.print((int)pitch);
    Serial.print(", z:"); Serial.println((int)yaw);

    delay(500);
}
