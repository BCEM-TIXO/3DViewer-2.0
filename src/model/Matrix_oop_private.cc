#include "Matrix.h"
using namespace s21;
void Matrix::MemoryAlloc() {
  matrix_ = new float*[rows_];
  matrix_[0] = new float[(size_t)rows_ * (size_t)cols_]();
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = matrix_[0] + i * cols_;
  }
}

void Matrix::MemoryFree() noexcept {
  if (matrix_ != nullptr) delete[](matrix_[0]);
  delete[](matrix_);
}

bool Matrix::CheckSim(const Matrix& other) const noexcept {
  if (rows_ == other.rows_ && cols_ == other.cols_)
    return true;
  else
    return false;
}

bool Matrix::CheckMul(const Matrix& other) const noexcept {
  if (cols_ == other.rows_)
    return true;
  else
    return false;
}

bool Matrix::CheckQuad() const noexcept {
  if (cols_ == rows_)
    return true;
  else
    return false;
}

Matrix Matrix::GetMinor(int row, int col) noexcept {
  Matrix minor(rows_ - 1, cols_ - 1);
  for (int i = 0, k = 0; i < rows_; i++, k++) {
    if (i == row) i++;
    if (i == rows_) break;
    for (int j = 0, q = 0; j < cols_; j++, q++) {
      if (j == col) j++;
      if (j == cols_) break;
      minor(k, q) = matrix_[i][j];
    }
  }
  return minor;
}

void Matrix::Treangle() noexcept {
  int i, j, k;
  int swapsCount = 0;
  float q;
  for (i = 0; i < rows_; ++i) {
    // Поиск строки с максимальным первым элементом
    int rowMax = i;
    for (j = i + 1; j < rows_; ++j) {
      if (fabs(matrix_[j][i]) > fabs(matrix_[rowMax][i])) rowMax = j;
    }
    // Если нашли делаем перестановку (элементарное преобразование)
    if (rowMax != i) {
      std::swap_ranges(matrix_[i], matrix_[i] + cols_, matrix_[rowMax]);
      swapsCount++;
    }
    // Вычитаем из всех строк снизу текущую, так чтобы под элементом оказался 0
    for (j = i + 1; matrix_[i][i] != 0 && j < rows_; ++j) {
      if (fabs(matrix_[j][i]) < EPS) continue;
      q = -matrix_[j][i] / matrix_[i][i];
      for (k = rows_ - 1; k >= i; --k) {
        matrix_[j][k] += q * matrix_[i][k];
      }
    }
  }
  // Нечетное кол-во перестановок -> умножаем строчку на -1
  for (i = 0; swapsCount % 2 == 1 && i < rows_; ++i) {
    matrix_[0][i] = -matrix_[i][i];
  }
}

void Matrix::CopyFrom(const Matrix& other, int row, int col) noexcept {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}