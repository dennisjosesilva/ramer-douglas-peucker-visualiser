#pragma once

#include <QWidget>

class MainWidget;

class QCheckBox;
class QSpinBox;


class SimplificationPanelWidget : public QWidget
{
Q_OBJECT

public:
  SimplificationPanelWidget(int npoints, MainWidget *parent=nullptr);

  void npointsSpinBox_onValueChanged(int val);
  void showContourCheckBox_stateChanged(int val);
  void showSimpleContourCheckBox_stateChanged(int val);

  void setMaxNPoints(int val);
  void setNPoints(int val);
  void setShowContour(int val);
  void setShowSimpleContour(int val);
  
private:
  MainWidget *mainWidget_;

  QSpinBox *npointsSpinBox_;
  QCheckBox *showContourCheckBox_;
  QCheckBox *showSimpleContourCheckBox_;
};