#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "../main.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}