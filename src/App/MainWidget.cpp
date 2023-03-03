#include "App/MainWidget.hpp"
#include "Contour/ContourExtractor.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent)
  : QWidget{parent}
{
  const int NPOINTS = 300;

  layout_ = new QHBoxLayout;

  simpPanel_ = new SimplificationPanelWidget{NPOINTS, this};
  layout_->addWidget(simpPanel_);
  
  graphicsView_ = new GraphicsViewWidget{NPOINTS, this};
  layout_->addWidget(graphicsView_);
  
  setLayout(layout_);
}

void MainWidget::loadImage(const QString &filename)
{
  graphicsView_->loadImage(filename);
  simpPanel_->setMaxNPoints(graphicsView_->npoints());
  simpPanel_->setNPoints(graphicsView_->npoints());
  simpPanel_->setShowSimpleContour(false);
  simpPanel_->setShowContour(true);
  graphicsView_->update();
}
