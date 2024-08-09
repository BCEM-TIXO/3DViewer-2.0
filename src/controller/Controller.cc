#include "Controller.h"

namespace s21 {

bool Controller::loadModelFromFile(const std::string &filepath) {
  currentFileName_ = filepath;
  bool result = model_.LoadFromFile(filepath);
  if (result) {
    model_.Normalize();
  }
  return result;
}

const float *Controller::modelVertexsData() { return model_.vertexsData(); }

const unsigned *Controller::modelIndexesData() { return model_.indexsData(); }

size_t Controller::modelVertexsCount() { return model_.vertexsCount(); }

size_t Controller::modelFacesCount() { return model_.facesCount(); }

size_t Controller::modelLinesCount() { return model_.linesSize() / 2; }

size_t Controller::modelLinesSize() { return model_.linesSize(); };

std::vector<unsigned int>::const_iterator Controller::modelLinesCbegin() {
  return model_.linesCbegin();
};

void Controller::moveAx(AxisType axis, float distance) {
  model_.MoveOneAxis(axis, distance);
  Notify();
}

void Controller::rotateAx(AxisType axis, float angle) {
  model_.RotateOneAxis(axis, angle);
  Notify();
}

void Controller::scale(float scale) { model_.ScaleObject(scale); 
  Notify();
}

void Controller::Attach(IObserver *observer) { observers_.push_back(observer); }

void Controller::Detach(IObserver *observer) {
  observers_.erase(std::remove(observers_.begin(), observers_.end(), observer),
                   observers_.end());
}

void Controller::Notify() {
  for (IObserver *observer : observers_) {
    observer->UpdateModelInfo();
  }
}
} // namespace s21
