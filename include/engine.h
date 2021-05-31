#ifndef ENGINE_H
#define ENGINE_H


#define SATURATION_PWM_MAX 1900
#define SATURATION_PWM_MIN 1100

#include <stddef.h>
#include <stdint.h>

#include "politocean/serializable.h"
#include "Matlib.h"

using namespace matlib;

class ReferenceException {
  const char *_msg;

public:
  ReferenceException(const char *msg) : _msg(msg) {}
  const char *what() const { return _msg; }
};

class Reference {
  uint8_t _x, _y, _z, _roll, _pitch, _yaw;

public:
  Reference(uint8_t x, uint8_t y, uint8_t z, uint8_t roll, uint8_t pitch,
            uint8_t yaw)
      : _x(x), _y(y), _z(z), _roll(roll), _pitch(pitch), _yaw(yaw) {}
  Reference() : Reference(0, 0, 0, 0, 0, 0) {}

  inline uint8_t getX() { return _x; }
  inline uint8_t getY() { return _y; }
  inline uint8_t getZ() { return _z; }
  inline uint8_t getRoll() { return _roll; }
  inline uint8_t getPitch() { return _pitch; }
  inline uint8_t getYaw() { return _yaw; }

  inline void setX(uint8_t x) { _x = x; }
  inline void setY(uint8_t y) { _y = y; }
  inline void setZ(uint8_t z) { _z = z; }
  inline void setRoll(uint8_t roll) { _roll = roll; }
  inline void setPitch(uint8_t pitch) { _pitch = pitch; }
  inline void setYaw(uint8_t yaw) { _yaw = yaw; }

  inline void deserialize(const uint8_t *buffer, size_t size) {
    if (size != 6) {
      throw ReferenceException("Size don't match.");
    }

    _x = buffer[0];
    _y = buffer[1];
    _z = buffer[2];
    _roll = buffer[3];
    _pitch = buffer[4];
    _yaw = buffer[5];
  }

  void toPower(const double perc[6], double vect_Forces[6]);
};

class Engine {
public:
  void distribute(const double vect_Forces[6], Matrix<6,1> torques,
                        double y[7]);

  Matrix<6,1> addEnvironment(Matrix<6,1>,  double roll, double pitch, double yaw);

  int computePWM(double u);

  
};

#endif // ENGINE_H