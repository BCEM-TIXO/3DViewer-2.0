#ifndef CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_
#define CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
#define EPS 1e-7
namespace s21 {
class Matrix {
 public:
  Matrix();                             // default constructor
  explicit Matrix(int rows, int cols);  // parameterized constructor
  Matrix(const Matrix& other);          // copy construct
  Matrix(Matrix&& other) noexcept;      // move construct
  ~Matrix() noexcept;                   // destructor

  bool EqMatrix(const Matrix& other) const noexcept;
  void SumMatrix(const Matrix& other);
  void SubMatrix(const Matrix& other);

  void MulMatrix(const Matrix& other);

  Matrix Transpose();
  Matrix CalcComplements();

  int GetRows() const noexcept;
  int GetCols() const noexcept;

  void SetRows(int rows);
  void SetCols(int cols);

  float* data() { return *matrix_; };

  float& operator()(int row, int col);
  float operator()(int row, int col) const;
  Matrix& operator=(const Matrix& other);
  Matrix operator+(const Matrix& other) const;
  Matrix& operator+=(const Matrix& other);
  Matrix operator-(const Matrix& other) const;
  Matrix& operator-=(const Matrix& other);
  Matrix operator*(const Matrix& other) const;
  Matrix& operator*=(const Matrix& other);
  Matrix operator*(const float& num) const;
  bool operator==(const Matrix& other) const noexcept;

  void Print();
  void FillMatrix(std::vector<float> values);

 private:
  int rows_, cols_;
  float** matrix_;

  void MemoryAlloc();
  void MemoryFree() noexcept;

  bool CheckSim(const Matrix& other) const noexcept;
  bool CheckMul(const Matrix& other) const noexcept;
  bool CheckQuad() const noexcept;

  Matrix GetMinor(int row, int col) noexcept;
  void CopyFrom(const Matrix& other, int row, int col) noexcept;

  void Treangle() noexcept;
};
};      // namespace s21
#endif  // CPP1_S21_MATRIXPLUS_S21_MATRIX_OOP_H_
