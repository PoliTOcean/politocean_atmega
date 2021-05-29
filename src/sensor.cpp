#include <math.h>
#include <stdlib.h>

#include "politocean/constants.h"
#include "politocean/exception.h"
#include "politocean/sensor.h"

using namespace politocean;
using namespace politocean::exception;

Sensor::Sensor(const char *id) : _id(id), _value(10.5) {}

void Sensor::serialize(uint8_t *buffer, size_t size) {
  if (size != 3 * sizeof(char)) {
    throw SensorException("Size is not correct.");
  }

  double integral;
  double decimal;

  decimal = modf(_value, &integral);

  buffer[0] = politocean::constants::sensors::map(_id);
  buffer[1] = (uint8_t)integral;
  buffer[2] = (uint8_t)(decimal * 100);
}

void Sensor::deserialize(uint8_t *buffer, size_t size) {
  if (size != 3 * sizeof(char)) {
    throw SensorException("Size is not correct.");
  }

  if (politocean::constants::sensors::map(_id) != buffer[0]) {
    throw SensorException("IDs don't match during deserialization.");
  }

  _id = constants::sensors::map(buffer[0]);
  _value = (double)(buffer[1] + (double)buffer[2] / 100.0);
}