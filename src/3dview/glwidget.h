#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <memory>

#include "../controller/Controller.h"
#include "ProjectionStrategy.h"

class GLWidget : public QOpenGLWidget,
                 protected QOpenGLFunctions,
                 public s21::IObserver {
  Q_OBJECT

 public:
  GLWidget(QWidget *parent = nullptr);
  ~GLWidget();
  void initBuffers();
  void initSettings();
  void setProjectionStrategy(std::unique_ptr<s21::ProjectionStrategy> strategy);
  void UpdateModelInfo();

  QColor backgroundColor;
  QColor lineColor;
  QColor pointColor;
  int pointMode;
  int pointSize;
  int lineSize;
  int lineMode;
  int rotateMode;
  s21::Controller controller_;

 protected:
  void initializeGL() override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

 public slots:
  void setXMove(int step);
  void setYMove(int step);
  void setZMove(int step);
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);
  void setZoom(int zoom);

 signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);
  void xMoveChanged(int angle);
  void yMoveChanged(int angle);
  void zoomChanged(int zoom);

 private:
  QPoint m_lastPos;
  int m_coeffMatrixLoc = 0;
  int m_colorLoc = 0;
  int m_vertexLoc = 0;
  QOpenGLShaderProgram m_program;
  QOpenGLBuffer VertexBuff;
  QOpenGLBuffer indexBuff;
  QOpenGLVertexArrayObject VertexArrObj;
  QMatrix4x4 projectionMatrix;
  QMatrix4x4 moveMatrix;
  std::unique_ptr<s21::ProjectionStrategy> projectionStrategy_;

  int m_xRot = 0;
  int m_yRot = 0;
  int m_zRot = 0;
  int m_xMove = 0;
  int m_yMove = 0;
  int m_zMove = 0;
  float zoomVal = 100;
  int prev_xMove = 0;
  int prev_yMove = 0;
  int prev_zMove = 0;
  int prev_xRot = 0;
  int prev_yRot = 0;
  int prev_zRot = 0;
  float prev_zoomVal = 1;

  void clearBuffers();
  void cleanup();
  void updateBuffers();

  void drawPoints();
  void drawLines();
};

#endif  // GLWIDGET_H
