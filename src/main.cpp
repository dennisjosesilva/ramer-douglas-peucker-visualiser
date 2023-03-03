#include <QApplication>

#include "App/MainWindow.hpp"

int main(int argc, char *argv[])
{
  QApplication app{argc, argv};

  MainWindow mainWidnow;
  mainWidnow.resize(720, 680);

  mainWidnow.show();

  return app.exec();
}