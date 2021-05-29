#ifndef MPU6050_H
#define MPU6050_H

typedef struct {
  double x, y, z;
} cartesian_t;

class MPU6050 {
  cartesian_t _gyroErr = {.x = 0, .y = 0, .z = 0};
  cartesian_t _accErr = {.x = 0, .y = 0, .z = 0};

  void _calcError();

public:
  MPU6050();

  void readAccelerometer(cartesian_t *values);
  void readTemperature(int *temp);
  void readGyroscope(cartesian_t *values);
};

#endif // MPU6050_H