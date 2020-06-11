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
    //QProcess::startDetached("explorer yourPath");
//    QFileDialog dialog;
//    QDir currentDir = QDir::currentPath();
//    currentDir.cd("../../../files/InputFiles/Circuits");

//    dialog.setDirectory(currentDir);
//    dialog.setFileMode(QFileDialog::AnyFile);
//    dialog.exec();


    return a.exec();
}
