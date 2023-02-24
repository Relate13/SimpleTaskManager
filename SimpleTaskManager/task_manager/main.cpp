#include "mainwindow.h"
#include <QTimer>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon icon(":/icon.png");
    MainWindow w;
    w.setWindowTitle("Task Manager");
    w.setWindowIcon(icon);
    w.show();
    w.operate();
    return a.exec();
}
