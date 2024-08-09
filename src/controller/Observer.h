#ifndef CPP4_3DVIEWER_V2_0_CONTROLLER_OBSERVER_H_
#define CPP4_3DVIEWER_V2_0_CONTROLLER_OBSERVER_H_

namespace s21 {

class IObserver {
 public:
  virtual ~IObserver() = default;
  virtual void UpdateModelInfo() = 0;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_CONTROLLER_OBSERVER_H_
