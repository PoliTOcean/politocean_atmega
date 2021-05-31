#include <Arduino.h>
#include <Wire.h>

#include "MPU6050.h"

const int MPU6050_ADDR = 0x68;
const int MPU6050_ACCEL_XOUT_H = 0x3b;
const int MPU6050_TEMP_OUT_H = 0x41;
const int MPU6050_GYRO_XOUT_H = 0x43;
const double MPU6050_ACC_SCALE = 16384.0;
const double MPU6050_GYRO_SCALE = 131.0;
const cartesian_t CARTESIAN_ZERO = {.x = 0, .y = 0, .z = 0};

MPU6050::MPU6050() : _accErr(CARTESIAN_ZERO), _gyroErr(CARTESIAN_ZERO) {}

void MPU6050::begin() {
  Wire.begin();

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6b);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C); // Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(
      0x10); // Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1B); // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10); // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);

  delay(20);
}

void MPU6050::readAccelerometer(cartesian_t *values) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true);

  values->x = (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE - _accErr.x;
  values->y = (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE - _accErr.y;
  values->z = (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE - _accErr.z;
}

void MPU6050::readTemperature(int *temp) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_TEMP_OUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 2, true);

  *temp = (Wire.read() << 8 | Wire.read()) / 340.00 + 36.53;
}

void MPU6050::readGyroscope(cartesian_t *values) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_GYRO_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true);

  values->x =
      (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE - _gyroErr.x;
  values->y =
      (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE - _gyroErr.y;
  values->z =
      (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE - _gyroErr.z;
}

void MPU6050::calibrate() {
  int c = 0;
  while (c++ < 200) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 6, true);

    _accErr.x += (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE;
    _accErr.y += (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE;
    _accErr.z += (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE;
  }
  _accErr.x /= 200;
  _accErr.y /= 200;
  _accErr.z /= 200;

  c = 0;
  while (c++ < 200) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_GYRO_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 6, true);

    _gyroErr.x += (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE;
    _gyroErr.y += (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE;
    _gyroErr.z += (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE;
  }
  _gyroErr.x /= 200;
  _gyroErr.y /= 200;
  _gyroErr.z /= 200;

  Serial.print("AccErrorX: ");
  Serial.println(_accErr.x);
  Serial.print("AccErrorY: ");
  Serial.println(_accErr.y);
  Serial.print("AccErrorY: ");
  Serial.println(_accErr.z);
  Serial.print("GyroErrorX: ");
  Serial.println(_gyroErr.x);
  Serial.print("GyroErrorY: ");
  Serial.println(_gyroErr.y);
  Serial.print("GyroErrorZ: ");
  Serial.println(_gyroErr.z);
}

void MPU6050::setAccelerometerOffset(cartesian_t *values) {
  _accErr.x = values->x;
  _accErr.y = values->y;
  _accErr.z = values->z;
}
void MPU6050::setGyroscopeOffset(cartesian_t *values) {
  _gyroErr.x = values->x;
  _gyroErr.y = values->y;
  _gyroErr.z = values->z;
}