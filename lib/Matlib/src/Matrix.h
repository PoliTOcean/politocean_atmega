#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

namespace matlib {

template <size_t rows, size_t cols = 1, typename T = double> class Matrix {
  T entries[rows][cols];

public:
  static const size_t kRows = rows;
  static const size_t kCols = cols;

  Matrix() {}
  Matrix<rows, cols, T>(const Matrix<rows, cols, T> &obj) { *this = obj; }
  template <typename... ARGS> Matrix(ARGS... args) { FillRowMajor(args...); }
  static Matrix<rows, cols, T> zeros();
  static Matrix<rows, cols, T> ones();
  static Matrix<rows, cols, T> eye();

  inline T operator()(unsigned int row, unsigned int col = 0) const {
    return entries[row][col];
  }

  inline T &operator()(unsigned int row, unsigned int col = 0) {
    return entries[row][col];
  }

  template <typename... TAIL> void FillRowMajor(T head, TAIL... tail);
  void FillRowMajor() {}
  Matrix<rows, cols, T> &Fill(const T &val);

  // Addition
  Matrix<rows, cols, T> operator+(const Matrix<rows, cols, T> &obj) const;
  Matrix<rows, cols, T> &operator+=(const Matrix<rows, cols, T> &obj);

  // Subtraction
  Matrix<rows, cols, T> operator-(const Matrix<rows, cols, T> &obj) const;
  Matrix<rows, cols, T> &operator-=(const Matrix<rows, cols, T> &obj);

  // Negation
  Matrix<rows, cols, T> operator-() const;

  // Multiplication
  template <size_t opCols>
  Matrix<rows, opCols, T>
  operator*(const Matrix<cols, opCols, T> &operand) const;

  Matrix<rows, cols, T> &operator*=(const Matrix<rows, cols, T> &operand);

  // Element-wise

  Matrix<rows, cols, T> operator+(const T k) const;
  Matrix<rows, cols, T> operator-(const T k) const;
  Matrix<rows, cols, T> operator*(const T k) const;
  Matrix<rows, cols, T> operator/(const T k) const;

  Matrix<rows, cols, T> &operator+=(const T k);
  Matrix<rows, cols, T> &operator-=(const T k);
  Matrix<rows, cols, T> &operator*=(const T k);
  Matrix<rows, cols, T> &operator/=(const T k);
};

template <size_t rows, size_t cols, typename T>
template <typename... TAIL>
void Matrix<rows, cols, T>::FillRowMajor(T head, TAIL... tail) {
  static_assert(rows * cols > sizeof...(TAIL),
                "Too many arguments passed to FillRowMajor");
  (*this)((rows * cols - sizeof...(TAIL) - 1) / cols,
          (rows * cols - sizeof...(TAIL) - 1) % cols) = head;
  FillRowMajor(tail...);
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &Matrix<rows, cols, T>::Fill(const T &val) {
  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      (*this)(i, j) = val;

  return *this;
}

/****************************************************
 ********************* Addition *********************
 ***************************************************/

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T>
Matrix<rows, cols, T>::operator+(const Matrix<rows, cols, T> &obj) const {
  Matrix<rows, cols, T> ret;
  Add(*this, obj, ret);

  return ret;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &
Matrix<rows, cols, T>::operator+=(const Matrix<rows, cols, T> &obj) {
  Add(*this, obj, *this);

  return *this;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &Add(const Matrix<rows, cols, T> &A,
                           const Matrix<rows, cols, T> &B,
                           Matrix<rows, cols, T> &C) {
  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      C(i, j) = A(i, j) + B(i, j);

  return C;
}

/****************************************************
 ******************** Subtraction *******************
 ***************************************************/

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T>
Matrix<rows, cols, T>::operator-(const Matrix<rows, cols, T> &obj) const {
  Matrix<rows, cols, T> ret;
  Subtract(*this, obj, ret);

  return ret;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &
Matrix<rows, cols, T>::operator-=(const Matrix<rows, cols, T> &obj) {
  Subtract(*this, obj, *this);

  return *this;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &Subtract(const Matrix<rows, cols, T> &A,
                                const Matrix<rows, cols, T> &B,
                                Matrix<rows, cols, T> &C) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      C(i, j) = A(i, j) - B(i, j);

  return C;
}

/****************************************************
 ********************** Negation ********************
 ***************************************************/
template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator-() const {
  Matrix<rows, cols, T> ret;

  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      ret(i, j) = -(*this)(i, j);

  return ret;
}

/****************************************************
 ****************** Multiplication ******************
 ***************************************************/

template <size_t rows, size_t cols, typename T>
template <size_t opCols>
Matrix<rows, opCols, T>
Matrix<rows, cols, T>::operator*(const Matrix<cols, opCols, T> &operand) const {
  Matrix<rows, opCols, T> ret;
  Multiply(*this, operand, ret);

  return ret;
}

template <size_t rows, size_t cols, size_t opCols, typename T>
Matrix<rows, opCols, T> &Multiply(const Matrix<rows, cols, T> &A,
                                  const Matrix<cols, opCols, T> &B,
                                  Matrix<rows, opCols, T> &C) {
  unsigned int i, j, k;

  for (i = 0; i < rows; i++)
    for (j = 0; j < opCols; j++) {
      if (cols > 0)
        C(i, j) = A(i, 0) * B(0, j);

      for (k = 1; k < cols; k++)
        C(i, j) += A(i, k) * B(k, j);
    }

  return C;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::zeros() {
  Matrix<rows, cols, T> m;
  m.Fill(0);

  return m;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::ones() {
  Matrix<rows, cols, T> m;
  m.Fill(1);

  return m;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::eye() {
  Matrix<rows, cols, T> m;

  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      m(i, j) = (i == j) ? 1 : 0;

  return m;
}

/****************************************************
 ******************* Element-wise *******************
 ***************************************************/

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator+(const T k) const {
  Matrix<rows, cols, T> ret;
  ElementwiseAdd(*this, k, ret);

  return ret;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &Matrix<rows, cols, T>::operator+=(const T k) {
  ElementwiseAdd(*this, k, *this);

  return *this;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator-(const T k) const {
  Matrix<rows, cols, T> ret;
  ElementwiseSubtract(*this, k, ret);

  return ret;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &Matrix<rows, cols, T>::operator-=(const T k) {
  ElementwiseSubtract(*this, k, *this);

  return *this;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator*(const T k) const {
  Matrix<rows, cols, T> ret;
  ElementwiseMultiply(*this, k, ret);

  return ret;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &Matrix<rows, cols, T>::operator*=(const T k) {
  ElementwiseMultiply(*this, k, *this);

  return *this;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator/(const T k) const {
  Matrix<rows, cols, T> ret;
  ElementwiseDivide(*this, k, ret);

  return ret;
}

template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &Matrix<rows, cols, T>::operator/=(const T k) {
  ElementwiseDivide(*this, k, *this);

  return *this;
}

// Multiplies two matrices and stores the result in a third matrix C, this is
// slightly faster than using the operators
template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &ElementwiseAdd(const Matrix<rows, cols, T> &A,
                                      const T &B, Matrix<rows, cols, T> &C) {
  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      C(i, j) = A(i, j) + B;

  return C;
}

// Multiplies two matrices and stores the result in a third matrix C, this is
// slightly faster than using the operators
template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &ElementwiseSubtract(const Matrix<rows, cols, T> &A,
                                           const T &B,
                                           Matrix<rows, cols, T> &C) {
  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      C(i, j) = A(i, j) - B;

  return C;
}

// Multiplies two matrices and stores the result in a third matrix C, this is
// slightly faster than using the operators
template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &ElementwiseMultiply(const Matrix<rows, cols, T> &A,
                                           const T &B,
                                           Matrix<rows, cols, T> &C) {
  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      C(i, j) = A(i, j) * B;

  return C;
}

// Multiplies corresponding elements of two matrices  and stores result in third
// matrix C
template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &ElementwiseMultiply(const Matrix<rows, cols, T> &A,
                                           const Matrix<rows, cols, T> &B,
                                           Matrix<rows, cols, T> &C) {
  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      C(i, j) = A(i, j) * B(i, j);

  return C;
}

// Multiplies two matrices and stores the result in a third matrix C, this is
// slightly faster than using the operators
template <size_t rows, size_t cols, typename T>
Matrix<rows, cols, T> &ElementwiseDivide(const Matrix<rows, cols, T> &A,
                                         const T &B, Matrix<rows, cols, T> &C) {
  for (unsigned int i = 0; i < rows; i++)
    for (unsigned int j = 0; j < cols; j++)
      C(i, j) = A(i, j) / B;

  return C;
}

} // namespace matlib

#endif // MATRIX_H