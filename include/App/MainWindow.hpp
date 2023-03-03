#pragma once

#include <QMainWindow>

class MainWidget;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
  MainWindow();

private:
  // ==============================================
  // METHODS 
  // ==============================================
  void createMenus();
  void openImageAct_onAction();

  // ===============================================
  // ATTRIBUTES
  // ===============================================
  MainWidget *mainWidget_;

};