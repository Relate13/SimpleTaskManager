#include "flappy_face.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Flappy_face w;
    w.setWindowTitle("Flappy Face");
    w.show();
    w.start();
    return a.exec();
}
