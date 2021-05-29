#include "Arduino.h"
#include "Matlib.h"

using namespace matlib;

void print2(const Matrix<2> &a);

Matrix<2> input = {1, 1};
Integrator<2> integrator(
    Matrix<2>::zeros(),
    0.01); // define an integrator with initial state [0 0] and step 0.01

void setup() { Serial.begin(9600); }

void loop() {
  integrator.setInput(input);
  integrator.step();
  Matrix<2> newState = integrator.getState();
  print2(newState);
}

void print2(const Matrix<2> &a) {
  Serial.print(a(0)), Serial.print('\t'), Serial.println(a(1));
}