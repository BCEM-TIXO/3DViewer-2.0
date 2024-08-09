#include "glwidget.h"

#include <iostream>
GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { initSettings(); }

GLWidget::~GLWidget() { cleanup(); }

void GLWidget::initSettings() {
  moveMatrix.setToIdentity();
  moveMatrix.lookAt(QVector3D(0, 0, 2.), QVector3D(0, 0, 0),
                    QVector3D(0, 1, 0));
  backgroundColor.setRgb(0, 0, 0);
  lineColor.setRgb(0, 127, 51);
  lineSize = 1;
  lineMode = 0;
  pointColor.setRgb(0, 214, 120);
  pointSize = 1;
  pointMode = 0;
  controller_.Attach(this);
}

void GLWidget::clearBuffers() {
  if (VertexArrObj.isCreated()) {
    VertexArrObj.destroy();
  }
  if (VertexBuff.isCreated()) {
    VertexBuff.destroy();
  }
  if (indexBuff.isCreated()) {
    indexBuff.destroy();
  }
}

void GLWidget::cleanup() {
  m_program.release();
  makeCurrent();
  clearBuffers();
  doneCurrent();
  controller_.Detach(this);
}

void GLWidget::initBuffers() {
  clearBuffers();

  VertexBuff = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  VertexBuff.create();
  VertexBuff.bind();
  VertexBuff.setUsagePattern(QOpenGLBuffer::StaticDraw);
  VertexBuff.allocate(controller_.modelVertexsData(),
                      4 * controller_.modelVertexsCount() * sizeof(GLfloat));

  indexBuff = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  indexBuff.create();
  indexBuff.bind();
  indexBuff.setUsagePattern(QOpenGLBuffer::StaticDraw);
  indexBuff.allocate((void *)(&(*controller_.modelLinesCbegin())),
                     controller_.modelLinesSize() * sizeof(unsigned int));
  indexBuff.release();
  VertexBuff.release();
}

void GLWidget::updateBuffers() {
  VertexBuff.bind();
  VertexBuff.write(0, controller_.modelVertexsData(),
                   4 * controller_.modelVertexsCount() * sizeof(GLfloat));
  VertexBuff.release();
}

// Код веришнного шейдера на GLSL
static const char *vertexShaderSourceCore =
    // "#version 120\n"
    "attribute vec4 vertex;\n"
    "uniform mat4 projectionMatrix;\n"
    "void main() {\n"
    "   gl_Position = projectionMatrix * vertex;\n"
    "}\n";

// Код фрагментного шейдера на GLSL
static const char *fragmentShaderSourceCore =
    // "#version 120\n"
    "uniform vec4 color;\n"
    "void main() {\n"
    "   gl_FragColor = color;\n"
    "}\n";

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  // std::cout << "GL_SHADING_LANGUAGE_VERSION: " <<
  // glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  m_program.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                    vertexShaderSourceCore);
  m_program.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                    fragmentShaderSourceCore);
  m_program.link();
  m_program.bind();
  m_colorLoc = m_program.uniformLocation("color");
  m_vertexLoc = m_program.attributeLocation("vertex");
  m_coeffMatrixLoc = m_program.uniformLocation("projectionMatrix");

  m_program.release();
}

void GLWidget::drawPoints() {
  if (pointMode != 0) {
    m_program.setUniformValue(m_colorLoc, pointColor);
    glPointSize(pointSize);
    if (pointMode == 1) {
      glEnable(GL_POINT_SMOOTH);
    }
    if (pointMode == 2) {
      glDisable(GL_POINT_SMOOTH);
    }
    glDrawArrays(GL_POINTS, 0, controller_.modelVertexsCount());
  }
}

void GLWidget::drawLines() {
  m_program.setUniformValue(m_colorLoc, lineColor);

  glLineWidth(lineSize);
  if (lineMode == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00F0);
  }

  glDrawElements(GL_LINES, controller_.modelLinesSize(), GL_UNSIGNED_INT, 0);
  if (lineMode == 1) {
    glDisable(GL_LINE_STIPPLE);
  }
}

void GLWidget::paintGL() {
  glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
               backgroundColor.blueF(), backgroundColor.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (VertexBuff.isCreated()) {
    m_program.bind();
    VertexBuff.bind();
    indexBuff.bind();

    projectionStrategy_->project(projectionMatrix, width(), height());

    m_program.enableAttributeArray(m_vertexLoc);
    m_program.setAttributeBuffer(m_vertexLoc, GL_FLOAT, 0, 4);
    m_program.setUniformValue(m_coeffMatrixLoc, projectionMatrix * moveMatrix);
    drawPoints();
    drawLines();
    // indexBuff.unmap();
    VertexBuff.release();
    indexBuff.release();
    m_program.release();
  }
}

void GLWidget::UpdateModelInfo() {
  updateBuffers();
  update();
}

void GLWidget::setProjectionStrategy(
    std::unique_ptr<s21::ProjectionStrategy> strategy) {
  projectionStrategy_ = std::move(strategy);
}

int NormalizeAngle(int angle) {
  while (angle < -180) angle += 360;
  while (angle > 180) angle -= 360;
  return angle;
}

void GLWidget::setXMove(int step) {
  m_xMove = step - prev_xMove;
  prev_xMove = step;
  controller_.moveAx(s21::AxisType::X, 0.05 * m_xMove);
  emit xMoveChanged(step);
}

void GLWidget::setYMove(int step) {
  m_yMove = step - prev_yMove;
  prev_yMove = step;
  controller_.moveAx(s21::AxisType::Y, 0.05 * m_yMove);
  emit yMoveChanged(step);
}

void GLWidget::setZMove(int step) {
  m_zMove = step - prev_zMove;
  prev_zMove = step;
  controller_.moveAx(s21::AxisType::Z, 0.05 * m_zMove);
  // emit zMoveChanged(step);
}

void GLWidget::setXRotation(int angle) {
  angle = NormalizeAngle(angle);
  m_xRot = angle - prev_xRot;
  prev_xRot = angle;
  controller_.rotateAx(s21::AxisType::X, m_xRot);
  // std::cout << m_xRot << "\n";
  emit xRotationChanged(angle);
}

void GLWidget::setYRotation(int angle) {
  angle = NormalizeAngle(angle);
  m_yRot = angle - prev_yRot;
  prev_yRot = angle;
  controller_.rotateAx(s21::AxisType::Y, m_yRot);
  emit yRotationChanged(angle);
}

void GLWidget::setZRotation(int angle) {
  angle = NormalizeAngle(angle);
  m_zRot = angle - prev_zRot;
  prev_zRot = angle;
  controller_.rotateAx(s21::AxisType::Z, m_zRot);
  emit zRotationChanged(angle);
}

void GLWidget::setZoom(int zoom) {
  zoomVal = fabs(zoom / prev_zoomVal);
  prev_zoomVal = zoom;
  controller_.scale(zoomVal);
  emit zoomChanged(zoom);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  m_lastPos = event->position().toPoint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->position().toPoint().x() - m_lastPos.x();
  int dy = event->position().toPoint().y() - m_lastPos.y();
  if (event->buttons() & Qt::LeftButton) {
    setXRotation(prev_xRot + dy);
    setYRotation(prev_yRot + dx);
  } else if (event->buttons() & Qt::RightButton) {
    setZRotation(prev_zRot - dx);
  } else if (event->buttons() & Qt::MiddleButton) {
    setXMove(prev_xMove - dx);
    setYMove(prev_yMove + dy);
  }
  m_lastPos = event->position().toPoint();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  const int zoomStep = 5;
  int delta = event->angleDelta().y();

  int newZoomPercent = prev_zoomVal + (delta > 0 ? zoomStep : -zoomStep);

  const int minZoomPercent = 10;
  const int maxZoomPercent = 1000;
  newZoomPercent =
      std::max(minZoomPercent, std::min(newZoomPercent, maxZoomPercent));

  setZoom(newZoomPercent);
}
