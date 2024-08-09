#include "Matrix.h"
using namespace s21;

Matrix::Matrix() : rows_(3), cols_(3) { MemoryAlloc(); }

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows < 1 || cols < 1)
    throw std::invalid_argument("Matrix::Matrix: Matrix size incorrect");
  MemoryAlloc();
}

Matrix::Matrix(const Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  MemoryAlloc();
  std::copy(other.matrix_[0], other.matrix_[0] + rows_ * cols_, matrix_[0]);
}

Matrix::Matrix(Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.matrix_ = nullptr;
  other.cols_ = 0;
  other.rows_ = 0;
}

Matrix::~Matrix() { MemoryFree(); }

bool Matrix::EqMatrix(const Matrix& other) const noexcept {
  bool IsEq = CheckSim(other);
  for (int i = 0; IsEq && i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (fabs(matrix_[i][j] - other.matrix_[i][j]) > EPS) {
        IsEq = false;
      }
    }
  }
  return IsEq;
}

void Matrix::SumMatrix(const Matrix& other) {
  if (CheckSim(other) == false)
    throw std::invalid_argument(
        "Matrix::SumMatrix: Matrices have different size");
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void Matrix::SubMatrix(const Matrix& other) {
  if (CheckSim(other) == false)
    throw std::invalid_argument(
        "Matrix::SubMatrix: Matrices have different size");
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void Matrix::MulMatrix(const Matrix& other) {
  if (CheckMul(other) == false)
    throw std::invalid_argument(
        "Matrix::MulMatrix: Collums first matrix do not equal rows second");
  Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      for (int k = 0; k < other.rows_; k++) {
        result(i, j) += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = result;
}

Matrix Matrix::Transpose() {
  Matrix tmp(cols_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      tmp(j, i) = matrix_[i][j];
    }
  }
  return tmp;
}

Matrix Matrix::CalcComplements() {
  if (CheckQuad() == false)
    throw std::logic_error("Matrix::CalcComplements: Matrix is not square");
  Matrix result(rows_, cols_);
  if (rows_ != 1) {
    Matrix MinorMatrix(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        MinorMatrix = GetMinor(i, j);
        // result(i, j) = sign * MinorMatrix.Determinant();
      }
    }
  } else {
    result(0, 0) = 1;
  }
  return result;
}

int Matrix::GetRows() const noexcept { return rows_; }

int Matrix::GetCols() const noexcept { return cols_; }

void Matrix::SetRows(int rows) {
  if (rows < 1) {
    throw std::invalid_argument("Matrix::SetRows: Matrix size incorrect");
  } else if (rows > rows_) {
    Matrix tmp(rows, cols_);
    tmp.CopyFrom(*this, rows_, cols_);
    *this = Matrix(tmp);
  } else if (rows < rows_) {
    rows_ = rows;
  }
}

void Matrix::SetCols(int cols) {
  if (cols < 1) {
    throw std::invalid_argument("Matrix::SetCols: Matrix size incorrect");
  } else if (cols > cols_) {
    Matrix tmp(rows_, cols);
    tmp.CopyFrom(*this, rows_, cols_);
    *this = Matrix(tmp);
  } else if (cols < cols_) {
    cols_ = cols;
  }
}

void Matrix::Print() {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        std::cout << matrix_[i][j] << " ";
      }
      std::cout << std::endl;
    }
  };

  void Matrix::FillMatrix(std::vector<float> values) {
    for (size_t i = 0; i < (size_t)rows_; ++i) {
      for (size_t j = 0; j < (size_t)cols_; ++j) {
        if ((i + 1) * (j + 1) > values.size()) {
          return;
        }
        matrix_[i][j] = values[i * cols_ + j];
      }
    }
  };