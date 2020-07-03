#include "mainwindow.h"
#include <QApplication>
#include <QDesktopServices>
#include <QProcess>
#include <QUrl>
#include <QFileDialog>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
