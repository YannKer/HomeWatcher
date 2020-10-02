#include "mainwindow.h"
#include <QApplication>
#include "UDP_Server/UDP_Server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UDP_Server::init();

    MainWindow w;
    w.show();
    int retCode =  a.exec();

    UDP_Server::shutdown();


     return retCode;
}
