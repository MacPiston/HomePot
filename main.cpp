#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("HomePot Beta");
    w.show();
    return a.exec();
}
/*
 * TODO SECTION:
 * - fix layout
 * - fix File menu (no exit option etc.)
 * - fix icon background
 * - add necessary things like smartpointers etc
 * - memory leaks?
 */
