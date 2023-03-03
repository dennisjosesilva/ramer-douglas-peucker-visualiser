#pragma once 

#include <QWidget>
#include <QLayout>

#include "App/GraphicsViewWidget.hpp"
#include "App/SimplificationPanelWidget.hpp"

class MainWidget : public QWidget
{
Q_OBJECT
public:
  MainWidget(QWidget *parent=nullptr);

  GraphicsViewWidget *graphicsView() { return graphicsView_; }
  SimplificationPanelWidget *simpPanel() { return simpPanel_; }
  
  void loadImage(const QString &filename);

private:
  QLayout *layout_;
  GraphicsViewWidget *graphicsView_;
  SimplificationPanelWidget *simpPanel_;
};