#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("BigBlackDuck");
    a.setApplicationDisplayName("LineFunnel");
    a.setApplicationName("Line Funnel");
    a.setApplicationVersion("1.0 Alpha");
    MainWindow w;
    w.show();
    return a.exec();
}
