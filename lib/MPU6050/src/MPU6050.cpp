#include <Arduino.h>
#include <Wire.h>

#include "MPU6050.h"

const int MPU6050_ADDR = 0x68;
const int MPU6050_ACCEL_XOUT_H = 0x3b;
const int MPU6050_TEMP_OUT_H = 0x41;
const int MPU6050_GYRO_XOUT_H = 0x43;
const double MPU6050_ACC_SCALE = 16384.0;
const double MPU6050_GYRO_SCALE = 131.0;

MPU6050::MPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6b);
  Wire.write(0x00);
  Wire.endTransmission(true);

  delay(20);
}

void MPU6050::readAccelerometer(cartesian_t *values) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true);

  values->x = (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE;
  values->y = (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE;
  values->z = (Wire.read() << 8 | Wire.read()) / MPU6050_ACC_SCALE;
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
      (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE + _gyroErr.x;
  values->y =
      (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE + _gyroErr.y;
  values->z =
      (Wire.read() << 8 | Wire.read()) / MPU6050_GYRO_SCALE + _gyroErr.z;
}

void MPU6050::_calcError() {
  cartesian_t acc, gyro;

  int c = 0;
  while (c++ < 200) {
    readAccelerometer(&acc);
    _accErr.x +=
        ((atan((acc.y) / sqrt(pow((acc.x), 2) + pow((acc.z), 2))) * 180 / PI));
    _accErr.y +=
        ((atan(-1 * (acc.x) / sqrt(pow((acc.y), 2) + pow((acc.z), 2))) * 180 /
          PI));
  }
  _accErr.x /= 200;
  _accErr.y /= 200;

  c = 0;
  while (c++ < 200) {
    readGyroscope(&gyro);
    _gyroErr.x += gyro.x / MPU6050_GYRO_SCALE;
    _gyroErr.y += gyro.y / MPU6050_GYRO_SCALE;
    _gyroErr.z += gyro.z / MPU6050_GYRO_SCALE;
  }
  _gyroErr.x /= 200;
  _gyroErr.y /= 200;
  _gyroErr.z /= 200;
}