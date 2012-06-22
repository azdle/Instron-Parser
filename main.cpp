#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("The Barrett Data Toolbox");
    w.setWindowIcon(QIcon(":/images/toolbox.ico"));
    w.show();
    
    return a.exec();
}
