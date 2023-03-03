#include "App/SimplificationPanelWidget.hpp"
#include "App/MainWidget.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

#include <QSpinBox>
#include <QCheckBox>

SimplificationPanelWidget::SimplificationPanelWidget(
  int npoints, MainWidget *parent) : QWidget{parent}, mainWidget_{parent}
{
  QLabel *label = new QLabel{"Simplification", this};
  QVBoxLayout *layout = new QVBoxLayout;

  layout->setAlignment(Qt::AlignTop);

  label->setStyleSheet(
    "font-weight: bold; text-decoration: underline;");

  layout->addWidget(label);

  npointsSpinBox_ = new QSpinBox{this};
  npointsSpinBox_->setRange(0, npoints);
  npointsSpinBox_->setValue(npoints);
  npointsSpinBox_->setEnabled(false);
  connect(npointsSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, 
    &SimplificationPanelWidget::npointsSpinBox_onValueChanged);

  QHBoxLayout *npointsLayout = new QHBoxLayout;
  npointsLayout->addWidget(new QLabel{"Number of points: ", this});
  npointsLayout->addWidget(npointsSpinBox_);
  layout->addItem(npointsLayout);

  showContourCheckBox_ = new QCheckBox{"Show contour", this};
  showContourCheckBox_->setChecked(true);
  connect(showContourCheckBox_, &QCheckBox::stateChanged, this,
    &SimplificationPanelWidget::showContourCheckBox_stateChanged);
  layout->addWidget(showContourCheckBox_);

  showSimpleContourCheckBox_ = new QCheckBox{"Show simple contour", this};
  showSimpleContourCheckBox_->setChecked(false);
  connect(showSimpleContourCheckBox_, &QCheckBox::stateChanged, this,
    &SimplificationPanelWidget::showSimpleContourCheckBox_stateChanged);
  layout->addWidget(showSimpleContourCheckBox_);
  
  setLayout(layout);
}

void SimplificationPanelWidget::npointsSpinBox_onValueChanged(int val)
{
  mainWidget_->graphicsView()->simplifyContour(val);
}

void SimplificationPanelWidget::showContourCheckBox_stateChanged(int val)
{
  mainWidget_->graphicsView()->setShowContour(showContourCheckBox_->isChecked());
} 

void SimplificationPanelWidget::showSimpleContourCheckBox_stateChanged(int val)
{
  npointsSpinBox_->setEnabled(showSimpleContourCheckBox_->isChecked());
  mainWidget_->graphicsView()->setShowSimpleContour(showSimpleContourCheckBox_->isChecked());

  if (showSimpleContourCheckBox_->isChecked())
    mainWidget_->graphicsView()->simplifyContour(npointsSpinBox_->value());

}

void SimplificationPanelWidget::setMaxNPoints(int val)
{
  npointsSpinBox_->setMaximum(val);
}

void SimplificationPanelWidget::setNPoints(int val)
{
  npointsSpinBox_->setValue(val);
}

void SimplificationPanelWidget::setShowContour(int val)
{
  showContourCheckBox_->setChecked(val);
}

void SimplificationPanelWidget::setShowSimpleContour(int val)
{
  showSimpleContourCheckBox_->setChecked(val);
}

