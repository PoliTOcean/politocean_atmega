#include "politocean/serializable.h"
#include <stddef.h>
#include <stdint.h>

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

  void deserialize(const uint8_t *buffer, size_t size);
};