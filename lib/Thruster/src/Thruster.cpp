#include "Thruster.h"

static const unsigned int kThrusterStopValue = 1500;
static const unsigned int kThrusterMaxValue = 1900;
static const unsigned int kThrusterMinValue = 1100;

long saturate(long value, long minValue, long maxValue);
long map(long x, long in_min, long in_max, long out_min, long out_max);

void Thruster::attach(uint8_t pin) {
  if (_pin != 0) {
    return;
  }

  _pin = pin;

  servo.attach(_pin);
  servo.writeMicroseconds(kThrusterStopValue);
}

void Thruster::detach() {
  _pin = 0;
  servo.detach();
}

void Thruster::write(int value) {
  value = saturate(value, kThrusterMinValue, kThrusterMaxValue);

  servo.writeMicroseconds(value);
}

void Thruster::setValue(int value) {
  value = saturate(value, _minVal, _maxVal);
  value = map(value, _minVal, _maxVal, kThrusterMinValue, kThrusterMaxValue);

  write(value);
}

void Thruster::stop() { write(kThrusterStopValue); }

long saturate(long value, long minValue, long maxValue) {
  if (value > maxValue)
    return maxValue;
  if (value < minValue)
    return minValue;

  return value;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
