#include "mainwindow.h"
#include "localcontroller.h"
#include <QApplication>
#include "protocol.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Protocol p;
    LocalController lc(nullptr, &w, &p);
    w.show();
    return a.exec();
}
