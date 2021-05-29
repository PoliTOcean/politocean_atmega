#ifndef SENSOR_H
#define SENSOR_H

#include <stddef.h>
#include <stdint.h>

namespace politocean {
class Sensor {
  const char *_id;
  double _value;

public:
  Sensor(const char *id);

  inline const char *getID() const { return _id; }
  inline double getValue() const { return _value; }
  inline void setValue(double value) { _value = value; }

  void serialize(uint8_t *buffer, size_t size);
  void deserialize(uint8_t *buffer, size_t size);
};
} // namespace politocean

#endif // SENSOR_Hq