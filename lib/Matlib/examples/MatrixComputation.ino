#include "Arduino.h"
#include "Matlib.h"

using namespace matlib;

void print2by2(const Matrix<2, 2> &a);

Matrix<2, 2> a = {1, 2, 3, 4};
Matrix<2, 2> b = {1, 3, 2, 4};

void setup() { Serial.begin(9600); }

void loop() {
  Serial.println("A = ");
  print2by2(a);

  Serial.println("B = ");
  print2by2(b);

  Matrix<2, 2> c = a * b;
  Serial.println("C = ");
  print2by2(c);

  delay(10000);
}

void print2by2(const Matrix<2, 2> &a) {
  Serial.print(a(0)), Serial.print('\t'), Serial.println(a(1));
  Serial.print(a(0)), Serial.print('\t'), Serial.println(a(1));
}