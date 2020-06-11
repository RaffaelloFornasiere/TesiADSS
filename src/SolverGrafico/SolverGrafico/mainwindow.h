#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <thread>
#include <vector>
#include "solverdialog.h"
#include <stdlib.h>
#include <QDebug>

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
    void on_spinBox_PopSize_valueChanged(int arg1);
    void on_spinBox_Pe_valueChanged(int arg1);
    void on_doubleSpinBox_Rho_e_valueChanged(double arg1);
    void on_spinBox_Pm_valueChanged(int arg1);
    void on_spinBox_MaxGens_valueChanged(int arg1);
    void on_doubleSpinBox_MinDelay_valueChanged(double arg1);
    void on_spinBox_Deadlock_valueChanged(int arg1);
    void on_checkBox_MaxGens_stateChanged(int arg1);
    void on_checkBox_MinDelay_stateChanged(int arg1);
    void on_checkBox_Deadlock_stateChanged(int arg1);
    void on_radioButton_AndCriteria_clicked(bool checked);
    void on_radioButton_OrCriteria_clicked(bool checked);

    void on_pushButton_CreateSolver_clicked();

private:
    Ui::MainWindow *ui;

    //parameters
    size_t popSize;
    size_t pe;
    size_t pm;
    double rho_e;

    //stop criteria
    size_t maxGens;
    size_t deadlock;
    double minDelay;


    const bool criteriaMaxGens = 0;
    const bool criteriaMinDelay = 1;
    const bool criteriaDeadlock = 2;

    unsigned criteria;
    bool criteriaAndOr;

    std::vector<std::thread> solvers;



};
#endif // MAINWINDOW_H
