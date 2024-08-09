#include "Matrix.h"
using namespace s21;

Matrix& Matrix::operator=(const Matrix& other) {
  // Защита от самоприсваивания
  if (this == &other) {
    return *this;
  }
  if (CheckSim(other) == false) {
    MemoryFree();
    rows_ = other.rows_;
    cols_ = other.cols_;
    MemoryAlloc();
  }
  std::copy(other.matrix_[0], other.matrix_[0] + rows_ * cols_, matrix_[0]);
  return *this;
}

float& Matrix::operator()(int row, int col) {
  if (row >= rows_ || col >= cols_ || row < 0 || col < 0)
    throw std::out_of_range("Matrix::operator(): Invalid matrix indexs ");
  return matrix_[row][col];
}

float Matrix::operator()(int row, int col) const {
  if (row >= rows_ || col >= cols_ || row < 0 || col < 0)
    throw std::out_of_range("Matrix::operator(): Invalid matrix indexs");
  return matrix_[row][col];
}

Matrix Matrix::operator+(const Matrix& other) const {
  Matrix tmp = *this;
  tmp += other;
  return tmp;
}

Matrix& Matrix::operator+=(const Matrix& other) {
  SumMatrix(other);
  return *this;
}

Matrix Matrix::operator-(const Matrix& other) const {
  Matrix tmp = *this;
  tmp -= other;
  return tmp;
}

Matrix& Matrix::operator-=(const Matrix& other) {
  SubMatrix(other);
  return *this;
}

Matrix Matrix::operator*(const Matrix& other) const {
  Matrix tmp = *this;
  tmp *= other;
  return tmp;
}

Matrix& Matrix::operator*=(const Matrix& other) {
  MulMatrix(other);
  return *this;
}


bool Matrix::operator==(const Matrix& other) const noexcept {
  return EqMatrix(other);
}