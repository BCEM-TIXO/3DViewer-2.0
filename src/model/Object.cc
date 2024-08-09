#include "Object.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string getNumber(std::string::iterator& it,
                      std::string::const_iterator end) {
  size_t i = 0;
  bool dot = false;
  bool digit = false;
  while ((std::isdigit(*it) || *it == '.') && it != end) {
    if (std::isdigit(*it)) {
      digit = true;
    }
    if (dot == false && *it == '.') {
      dot = true;
    } else if (dot && *it == '.') {
      throw std::invalid_argument("Number with 2 dots");
    }
    ++i;
    ++it;
  }
  if (digit == false) {
    throw std::invalid_argument("Точка есть а цифр нет");
  }
  return std::string(&(*((it--) - i)), i);
}

s21::Object::Object(std::string file_path) : path_to_file(file_path) {}

void s21::Object::pushFace(std::string str, size_t pos, size_t vertexs_read) {
  auto it = str.begin();
  auto end = str.cend();
  while (it <= end) {
    if (*it == ' ') {
      ++it;
      continue;
    }
    auto token = getToken(it, end);
      if (token != "" && token != "/" && token != "-") {
        int index = std::stoll(token);
      if (index < 0) {
      faces_[pos].emplace_back(vertexs_read + index);
    } else {
      faces_[pos].emplace_back(index - 1);
    }
      }
  }
}

void s21::Object::pushVertex(const std::string& str, size_t pos) {
  std::istringstream stream(str);
  for (int i = 0; i < 3; ++i) {
    stream >> vertexs_(pos, i);
  }
  if (!(stream >> vertexs_(pos, 3))) {
    vertexs_(pos, 3) = 1;
  }
}

bool s21::Object::LoadFromFile(const std::string file_path) {
  std::fstream file(file_path, std::ios_base::in);
  if (file.is_open() == false) {
    return false;
  }
  size_t vertexs_count = 0, faces_count = 0;
  std::string line;
  while (std::getline(file, line)) {
    if (line.size() > 2) {
      auto substr = line.substr(0, 2);
      if (substr == "v ") {
        ++vertexs_count;
      } else if (substr == "f ") {
        ++faces_count;
      }
    }
  }
  file.clear();
  file.seekg(0, std::ios_base::beg);
  vertexs_ = Matrix(vertexs_count, 4);
  faces_ = decltype(faces_)(faces_count);
  size_t v = 0;
  size_t f = 0;
  while (std::getline(file, line)) {
    if (line.size() > 2) {
      auto substr = line.substr(0, 2);
      if (substr == "v ") {
        pushVertex(line.substr(2), v);
        ++v;
      } else if (substr == "f ") {
        pushFace(line.substr(2), f, v);
        ++f;
      }
    }
  }
  file.close();
  toSimpleLines();
  return true;
}

void s21::Object::toSimpleLines() {
  lines_.clear();
  for (auto& f : faces_) {
    if (f.size() == 2) {
      lines_.emplace_back(f[0]);
      lines_.emplace_back(f[2]);
      continue;
    }
    for (auto i = f.cbegin(); i != f.cend(); ++i) {
      if (i == f.cbegin()) {
        lines_.emplace_back(*i);
      } else {
        lines_.emplace_back(*i);
        lines_.emplace_back(*i);
      }
    }
    lines_.emplace_back(f.front());
  }
}

std::string s21::getToken(std::string::iterator& it,
                          std::string::const_iterator end) {
  size_t i = 0;
  while (it != end && (std::isdigit(*it) || *it == '/' || *it == '-')) {
    ++it;
    ++i;
  }
  return std::string(&(*it++) - i, i);
}

void s21::Object::MoveOneAxis(AxisType axis, float distance) {
  int shift = axis;

  for (int i = 0; i < vertexsCount(); ++i) {
    vertexs_(i, shift) += distance;
  }
  offset[shift] += distance;
}

void s21::Object::MoveToZero() {
  decltype(offset) tmp;
  tmp = offset;
  MoveOneAxis(AxisType::X, -offset[0]);
  MoveOneAxis(AxisType::Y, -offset[1]);
  MoveOneAxis(AxisType::Z, -offset[2]);
  offset = tmp;
}

void s21::Object::MoveBack() {
  decltype(offset) tmp;
  tmp = offset;
  MoveOneAxis(AxisType::X, offset[0]);
  MoveOneAxis(AxisType::Y, offset[1]);
  MoveOneAxis(AxisType::Z, offset[2]);
  offset = tmp;
}

void s21::Object::RotateOneAxis(AxisType axis, float angle) {
  MoveToZero();

  float angle_radian = 0.01745329251 * angle;
  float cosf = cos(angle_radian);
  float sinf = sin(angle_radian);

  float x, y, z;

  for (int i = 0; i < vertexsCount(); ++i) {
    x = vertexs_(i, 0);
    y = vertexs_(i, 1);
    z = vertexs_(i, 2);
    if (axis == AxisType::X) {
      vertexs_(i, 1) = cosf * y - sinf * z;
      vertexs_(i, 2) = sinf * y + cosf * z;
    } else if (axis == AxisType::Y) {
      vertexs_(i, 0) = cosf * x + sinf * z;
      vertexs_(i, 2) = -sinf * x + cosf * z;
    } else if (axis == AxisType::Z) {
      vertexs_(i, 0) = cosf * x - sinf * y;
      vertexs_(i, 1) = sinf * x + cosf * y;
    }
  }

  MoveBack();
}

void s21::Object::ScaleObject(float scale) {
  for (int i = 0; i < vertexsCount(); ++i) {
    for (int axis = 0; axis < 3; ++axis) {
      vertexs_(i, axis) *= scale;
    }
  }
  for (auto& ax : offset) {
    ax *= scale;
  }
}

void s21::Object::Normalize() {
  float max_length = vertexs_(0, 0) * vertexs_(0, 0) +
                     vertexs_(0, 1) * vertexs_(0, 1) +
                     vertexs_(0, 2) * vertexs_(0, 2);

  float tmp, x, y, z;
  for (int i = 1; i < vertexsCount(); ++i) {
    x = vertexs_(i, 0);
    y = vertexs_(i, 1);
    z = vertexs_(i, 2);
    tmp = x * x + y * y + z * z;
    if (tmp > max_length) {
      max_length = tmp;
    }
  }
  max_length = 1 / sqrtf(max_length);
  ScaleObject(max_length);
}