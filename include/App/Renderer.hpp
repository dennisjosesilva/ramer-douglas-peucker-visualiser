#pragma once 

#include <QOpenGLFunctions>

#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <memory>

#include "App/Renderer.hpp"

class Renderer
{
public:
  Renderer(QOpenGLFunctions *gl, float width=1.0f, float height=1.0f);

  void setRendererSize(float width, float height);
  void init(const QVector<float> &contour);

  void updateSimpleContourBuffers(const QVector<float> &simpContour);
  void updateContourBuffers(const QVector<float> &contour);

  void setShowSimpleContour(bool val) { showSimpleContour_ = val; }
  void setShowContour(bool val) { showContour_ = val; }

  void draw();

private:
  // ==============================
  // AUXILIARY METHODS
  // ==============================
  void initShaders();
  void initBuffers(const QVector<float> &contour);

  void updateProjectionMatrix();

  // ===============================
  // ATTRIBUTES
  // ===============================
  float width_;
  float height_;

  QOpenGLFunctions *gl_;

  QOpenGLShaderProgram shaderProgram_;

  QOpenGLVertexArrayObject contourVAO_;
  QOpenGLBuffer coordsContourVBO_;

  QOpenGLVertexArrayObject simpContourVAO_;
  QOpenGLBuffer coordsSimpContourVBO_;

  unsigned int numContourPoints_;
  unsigned int numSimpContourPoints_;

  unsigned int uniColor_;
  unsigned int uniProjection_;

  bool showContour_;
  bool showSimpleContour_;
};