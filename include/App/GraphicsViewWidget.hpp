#pragma once 

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>

#include <memory>

#include "App/Renderer.hpp"

class MainWidget;

class GraphicsViewWidget : public QOpenGLWidget                                                 
{
Q_OBJECT
public:
  GraphicsViewWidget(int npoints, MainWidget *parent=nullptr);
  ~GraphicsViewWidget() {}

  QSize sizeHint() const override { return QSize{720, 720}; }

  void simplifyContour(int npoints);

  void setShowContour(bool val) { renderer_->setShowContour(val); update(); }
  void setShowSimpleContour(bool val) { renderer_->setShowSimpleContour(val); update(); }

  int npoints() const { return npoints_; }

  void loadImage(const QString &filename);

protected:
  void initializeGL() override;
  void paintGL() override;

  //=============================
  // ATTRIBUTES
  // ============================
  MainWidget *mainWidget_;

  QOpenGLFunctions *gl_;
  int npoints_;

  std::unique_ptr<Renderer> renderer_;
  QVector<float> contour_;
};