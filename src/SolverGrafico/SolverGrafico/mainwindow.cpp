#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    for(auto& i : solvers)
        i.join();
    delete ui;
}


void MainWindow::on_pushButton_OpenCircuitFile_clicked()
{
    QFileDialog dialog(this);
    QDir currentDir = QDir::currentPath();
    currentDir.cd("../../../files/InputFiles/Circuits");

    dialog.setDirectory(currentDir);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fileName;
    if (dialog.exec())
        fileName = dialog.selectedFiles()[0];

    ui->lineEdit_CircuitPath->setText(fileName);
}

void MainWindow::on_pushButton_OpenCellLib_clicked()
{
    QFileDialog dialog(this);
    QDir currentDir = QDir::currentPath();
    currentDir.cd("../../../files/InputFiles/Cell_Libraries");

    dialog.setDirectory(currentDir);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fileName;
    if (dialog.exec())
        fileName = dialog.selectedFiles()[0];

    ui->lineEdit_CellLibPath->setText(fileName);
}

void MainWindow::on_spinBox_PopSize_valueChanged(int arg1)
{
    popSize = arg1;
}

void MainWindow::on_spinBox_Pe_valueChanged(int arg1)
{
    pe = arg1;
}


void MainWindow::on_spinBox_Pm_valueChanged(int arg1)
{
    pm = arg1;
}


void MainWindow::on_doubleSpinBox_Rho_e_valueChanged(double arg1)
{
    rho_e = arg1;
}

void MainWindow::on_spinBox_MaxGens_valueChanged(int arg1)
{
    maxGens = arg1;
}

void MainWindow::on_doubleSpinBox_MinDelay_valueChanged(double arg1)
{
    minDelay = arg1;
}

void MainWindow::on_spinBox_Deadlock_valueChanged(int arg1)
{
    deadlock = arg1;
}

void MainWindow::on_checkBox_MaxGens_stateChanged(int arg1)
{
    criteria &= ~(unsigned(1));
    criteria |= bool(arg1);
}

void MainWindow::on_checkBox_MinDelay_stateChanged(int arg1)
{
    criteria &= ~(unsigned(2));
    criteria |= bool(arg1)*2;
}

void MainWindow::on_checkBox_Deadlock_stateChanged(int arg1)
{
    criteria &= ~(unsigned(4));
    criteria |= bool(arg1)*4;
}

void MainWindow::on_radioButton_AndCriteria_clicked(bool checked)
{
    criteriaAndOr = !checked;
}

void MainWindow::on_radioButton_OrCriteria_clicked(bool checked)
{
    criteriaAndOr = checked;
}

void MainWindow::on_pushButton_CreateSolver_clicked()
{
    CreateNewSolver();
}

void MainWindow::CreateNewSolver()
{
    SolverDialog d;
    d.show();
    d.exec();
    if(system(NULL))
    {
        qDebug() << "shell available";
    }
    else
    {
         qDebug() << "no shell available";
    }


}
