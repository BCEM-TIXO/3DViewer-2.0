#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow *mainWindow = MainWindow::getInstance();
  mainWindow->show();
  int result = a.exec();
  delete mainWindow;
  return result;
}
