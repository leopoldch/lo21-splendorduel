#include "mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
