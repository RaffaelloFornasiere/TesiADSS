#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "qcustomplot.h"
#include <thread>
#include <vector>
#include "solverdialog.h"
#include <stdlib.h>
#include <QDebug>
#include <QFileDialog>
#include <QThread>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void CreateNewSolver();
private slots:
    void on_pushButton_OpenCircuitFile_clicked();
    void on_pushButton_OpenCellLib_clicked();

    void on_pushButton_CreateSolver_clicked();

private:

    Ui::MainWindow *ui;

    ProcessArgs GetProcessArgsFromUi();




};
#endif // MAINWINDOW_H
