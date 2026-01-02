#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define SDA_PIN 8
#define SCL_PIN 9
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

unsigned long lastRead = 0;
const unsigned long interval = 200;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(1000);
  Serial.println("Initializing BNO055...");

  if (!bno.begin()) {
    Serial.println("BNO055 not detected! Check wiring or I2C address.");
    while (1);
  }

  bno.setExtCrystalUse(true);
  Serial.println("BNO055 ready. Move device to calibrate...");
}

void tryReinit() {
  Serial.println("Attempting re-init of BNO055...");
  bno = Adafruit_BNO055(55, 0x28);
  delay(50);
  if (!bno.begin()) {
    Serial.println("Re-init failed.");
  } else {
    Serial.println("Re-init successful.");
    bno.setExtCrystalUse(true);
  }
}

void loop() {
  if (millis() - lastRead < interval) return;
  lastRead = millis();

  uint8_t sys, gyro, accel, mag;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  Serial.print("EULER -> Yaw: ");
  Serial.print(euler.x(), 2);
  Serial.print(" | Pitch: ");
  Serial.print(euler.y(), 2);
  Serial.print(" | Roll: ");
  Serial.println(euler.z(), 2);

  Serial.print("CAL -> SYS:");
  Serial.print(sys);
  Serial.print(" GYRO:");
  Serial.print(gyro);
  Serial.print(" ACCEL:");
  Serial.print(accel);
  Serial.print(" MAG:");
  Serial.println(mag);

  static int zeroCount = 0;
  if (fabs(euler.x()) < 0.001 && fabs(euler.y()) < 0.001 && fabs(euler.z()) < 0.001) {
    zeroCount++;
  } else {
    zeroCount = 0;
  }

  if (zeroCount >= 3) {
    Serial.println("Detected repeated zero readings — possible wiring or power issue!");
    tryReinit();
    zeroCount = 0;
  }
}

