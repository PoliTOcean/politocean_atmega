#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "Matrix.h"
#include <stddef.h>

namespace matlib {
template <size_t rows, size_t cols = 1> class Integrator {
  Matrix<rows, cols> _state, _input;
  double _dt;

public:
  static const size_t kRows = rows;
  static const size_t kCols = cols;

  Integrator(const Matrix<rows, cols> &state, double dt)
      : _state(state), _input(Matrix<rows, cols>::zeros()), _dt(dt) {}

  inline const Matrix<rows, cols> &getState() const { return _state; }

  void setInput(const Matrix<rows, cols> &input) { _input = input; }

  void step() { _state += _input * _dt; }
};
} // namespace matlib
#endif // INTEGRATOR_H