#include "App/Renderer.hpp"

Renderer::Renderer(QOpenGLFunctions *gl, float width, float height)
  : gl_{gl}, width_{width}, height_{height}, 
    showContour_{true}, showSimpleContour_{false}
{}

void Renderer::setRendererSize(float width, float height)
{
  width_ = 600;
  height_ = 600;
  updateProjectionMatrix();
}

void Renderer::initShaders()
{
  shaderProgram_.create();

  shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, 
    ":shaders/simple.vs");
  shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment,
    ":shaders/simple.fs");
  shaderProgram_.link();

  uniColor_ = shaderProgram_.uniformLocation("color");
  uniProjection_ = shaderProgram_.uniformLocation("projection");
  updateProjectionMatrix();
}

void Renderer::initBuffers(const QVector<float> &contour)
{
  numContourPoints_ = contour.count() / 2;

  // ==========================
  // Set up contour VAO and VBO
  // ==========================
  contourVAO_.create();
  contourVAO_.bind();
  shaderProgram_.bind();

  coordsContourVBO_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  coordsContourVBO_.create();
  coordsContourVBO_.bind();
  coordsContourVBO_.allocate(contour.data(), contour.count() * sizeof(float));
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shaderProgram_.enableAttributeArray(0);
  
  shaderProgram_.release();
  contourVAO_.release();
  
  // =========================================
  // Set up simple contour VAO and (empty) VBO
  // =========================================
  simpContourVAO_.create();
  simpContourVAO_.bind();
  shaderProgram_.bind();

  coordsSimpContourVBO_.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  coordsSimpContourVBO_.create();
  coordsSimpContourVBO_.bind();
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);
  shaderProgram_.enableAttributeArray(0);

  shaderProgram_.release();
  simpContourVAO_.release();
}


void Renderer::updateSimpleContourBuffers(const QVector<float> &simpContour)
{
  numSimpContourPoints_ = simpContour.count() / 2;
  simpContourVAO_.bind();
  coordsSimpContourVBO_.bind();
  coordsSimpContourVBO_.allocate(simpContour.data(), simpContour.count() * sizeof(float));
  simpContourVAO_.release();
  coordsSimpContourVBO_.release();
}

void Renderer::updateContourBuffers(const QVector<float> &contour)
{
  for (int i = 0; i < 20; i+=2) {
    qDebug() << "(" << contour[i] << ", " << contour[i+1] << ")";
  }


  numContourPoints_ = contour.count() / 2;
  contourVAO_.bind();
  coordsContourVBO_.bind();
  coordsContourVBO_.allocate(contour.data(), contour.count() * sizeof(float));
  contourVAO_.release();
  coordsContourVBO_.release();
}

void Renderer::updateProjectionMatrix()
{
  QMatrix4x4 proj;

  qDebug() << width_ << " " << height_;

  proj.ortho(0.0f, width_, 0.0f, height_, -1.0f, 1.0f);
  qDebug() << shaderProgram_.bind();
  shaderProgram_.setUniformValue(uniProjection_, proj);
  shaderProgram_.release();
}

void Renderer::init(const QVector<float> &contour)
{
  initShaders();
  initBuffers(contour);
}

void Renderer::draw()
{
  if (showContour_) {
    contourVAO_.bind();
    shaderProgram_.bind();
    
    shaderProgram_.setUniformValue(uniColor_, QVector3D{1.0f, 1.0f, 1.0f});
    gl_->glDrawArrays(GL_LINE_LOOP, 0, numContourPoints_);

    contourVAO_.release();
    shaderProgram_.release();
  }

  if (showSimpleContour_) {
    simpContourVAO_.bind();
    shaderProgram_.bind();
    
    shaderProgram_.setUniformValue(uniColor_, QVector3D{1.0f, 0.0f, 0.0f});
    gl_->glDrawArrays(GL_LINE_LOOP, 0, numSimpContourPoints_);

    simpContourVAO_.release();
    shaderProgram_.release();
  }
  
}
