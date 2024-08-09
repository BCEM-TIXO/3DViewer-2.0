#ifndef CPP4_3DVIEWER_V2_0_3DVIEW_PROJECTIONSTRATEGY_H_
#define CPP4_3DVIEWER_V2_0_3DVIEW_PROJECTIONSTRATEGY_H_

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

namespace s21 {
class ProjectionStrategy {
 public:
  virtual ~ProjectionStrategy() = default;
  virtual void project(QMatrix4x4 &projectionMatrix, int width, int height) = 0;
};

class ParallelProjection : public ProjectionStrategy {
 public:
  void project(QMatrix4x4 &projectionMatrix, int width, int height) override {
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-(float)width / height, (float)width / height, -1, 1,
                           -10, 10);
  }
};

class CentralProjection : public ProjectionStrategy {
 public:
  void project(QMatrix4x4 &projectionMatrix, int width, int height) override {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45, (float)width / height, 0.01, 100);
  }
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_3DVIEW_PROJECTIONSTRATEGY_H_
