#ifndef MPU6050_H
#define MPU6050_H

typedef struct {
  double x, y, z;
} cartesian_t;

class MPU6050 {
  cartesian_t _accErr, _gyroErr;

public:
  MPU6050();

  void begin();

  void readAccelerometer(cartesian_t *values);
  void readTemperature(int *temp);
  void readGyroscope(cartesian_t *values);

  void setAccelerometerOffset(cartesian_t *values);
  void setGyroscopeOffset(cartesian_t *values);

  void calibrate();
};

#endif // MPU6050_H