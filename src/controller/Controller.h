#ifndef CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_
#define CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_

#include "../model/Object.h"
#include "Observer.h"

namespace s21 {
class Controller {
 private:
  Object model_;
  std::vector<IObserver *> observers_;
  std::string currentFileName_;

 public:
  Controller() = default;
  ~Controller() = default;

  bool loadModelFromFile(const std::string &filepath);
  const float *modelVertexsData();
  const unsigned *modelIndexesData();
  size_t modelVertexsCount();
  size_t modelFacesCount();
  size_t modelLinesCount();
  size_t modelLinesSize();
  std::vector<unsigned int>::const_iterator modelLinesCbegin();

  void moveAx(AxisType axis, float distance);
  void rotateAx(AxisType axis, float angle);
  void scale(float scale);

  void Attach(IObserver *observer);

  void Detach(IObserver *observer);

  void Notify();
  const std::string &getCurrentFileName() const { return currentFileName_; }
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_
