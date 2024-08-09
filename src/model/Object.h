#ifndef CPP4_3DVIEWER_V2_0_MODEL_OBJECT_H_
#define CPP4_3DVIEWER_V2_0_MODEL_OBJECT_H_

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "Matrix.h"
namespace s21 {
std::string getToken(std::string::iterator &it,
                     std::string::const_iterator end);

enum class TrasformationType { Move, Rotate, Scale };

enum AxisType {
  X,
  Y,
  Z
};  // Кажется более уместным тк потом без проблем каститься в инт

enum class ProjectionType { Paralel, Central };

class Object {
 public:
  Object() = default;
  Object(std::string file_path);

  bool LoadFromFile(const std::string file_path);
  int vertexsCount() { return vertexs_.GetRows(); };
  size_t facesCount() { return faces_.size(); };

  const float *vertexsData() { return vertexs_.data(); }
  const unsigned *indexsData() { return lines_.data(); }
  size_t linesSize() { return lines_.size(); };
  auto linesCbegin() { return lines_.cbegin(); };

  void MoveOneAxis(AxisType axis, float distance);
  void RotateOneAxis(AxisType axis, float angle);
  void ScaleObject(float scale);
  void Normalize();

  Matrix *GetMatrix() { return &vertexs_; }

 private:
  std::string path_to_file;
  std::vector<std::vector<unsigned>> faces_;
  std::vector<unsigned> lines_;
  Matrix vertexs_;
  Matrix transform_ = Matrix(4, 4);
  Matrix projection_ = Matrix(4, 4);
  std::array<float, 3> offset = {
      0.0, 0.0, 0.0};  // В целом думаю без разницы вектор или эрей, но если
                       // что-то можно положить на стек, то положи на стек

  void toSimpleLines();
  void pushFace(std::string str, size_t pos, size_t vertexs_read);
  void pushVertex(const std::string &str, size_t pos);

  void MoveToZero();
  void MoveBack();
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_MODEL_OBJECT_H_