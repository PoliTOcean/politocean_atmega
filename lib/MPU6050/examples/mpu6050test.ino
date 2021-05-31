#include "MPU6050.h"
#include "Wire.h"

MPU6050 mpu;
cartesian_t acc, gyro;

void setup() {
  mpu.begin();
  mpu.calibrate();
}

void loop() {
  mpu.readAccelerometer(&acc);
  mpu.readGyroscope(&gyro);
}