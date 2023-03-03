#include "App/GraphicsViewWidget.hpp"
#include "Contour/ContourExtractor.hpp"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLFunctions>

#include <QtMath>
#include <QVector2D>

#include "App/MainWidget.hpp"

#include "lib/psimpl.h"

GraphicsViewWidget::GraphicsViewWidget(int npoints, MainWidget *parent)
  : QOpenGLWidget{parent}, npoints_{npoints}, mainWidget_{parent}
{
  setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void GraphicsViewWidget::initializeGL()
{
  gl_ = QOpenGLContext::currentContext()->functions();

  renderer_ = std::make_unique<Renderer>(gl_);

  // Initial Polygon 
  const float angleStep = (2.0f*M_PI) / static_cast<float>(npoints_);
  const float RADIUS = 0.25f;
  const QVector2D CENTER{0.5f, 0.5f};
  for (int i = 0; i < npoints_; i++) {
    float x = RADIUS * qCos(i*angleStep) + CENTER.x();
    float y = RADIUS * qSin(i*angleStep) + CENTER.y();
    contour_.append(x);
    contour_.append(y);
  }

  renderer_->init(contour_);
}

void GraphicsViewWidget::simplifyContour(int npoints)
{
  npoints_ = npoints;

  QVector<float> simplerContour;
  psimpl::simplify_douglas_peucker_n<2>(contour_.begin(), contour_.end(), 
    npoints_, std::back_inserter(simplerContour));
  
  renderer_->updateSimpleContourBuffers(simplerContour);
  update();
}

void GraphicsViewWidget::loadImage(const QString &filename)
{
  QImage img;
  if (img.load(filename) && img.allGray()) {
    QVector<bool> bimg(img.width() * img.height(), false);
    
    for (int y = 0; y < img.height(); y++) {
      for (int x = 0; x < img.width(); x++) {
        if (qGray(img.pixel(x, y)) <= 10) 
          bimg[y*img.width() + x] = true;
      }
    }

    makeCurrent();
    contour_ = extractContour(bimg, img.width(), img.height());

    int HH = img.height() / 2;
    int HW = img.width() / 2;

    // un-invert y-axis for rendering proposed.
    for (int i = 0; i < contour_.count(); i+=2) {
      contour_[i] = HW + contour_[i];
      contour_[i+1] = HH + (img.height() - contour_[i+1]);
    }

    npoints_ = contour_.count() / 2;
    renderer_->setShowContour(true);
    renderer_->setShowSimpleContour(false);
    renderer_->setRendererSize(
      static_cast<float>(img.width()), static_cast<float>(img.height()));
    renderer_->updateContourBuffers(contour_);
  }
}

void GraphicsViewWidget::paintGL()
{
  gl_->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  gl_->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderer_->draw();
}