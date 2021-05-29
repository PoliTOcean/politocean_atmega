#ifndef THRUSTER_H
#define THRUSTER_H

#include <Servo.h>

class Thruster {
  uint8_t _pin;
  const int _minVal, _maxVal;

  Servo servo;

public:
  Thruster(int minVal, int maxVal)
      : _pin(0), _minVal(minVal), _maxVal(maxVal){};

  void attach(uint8_t pin);
  void detach();

  void setValue(int value);
  void write(int value);
  void stop();
};

#endif // THRUSTER_H