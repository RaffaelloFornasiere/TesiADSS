#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDir d;
    if(d.exists("../../../files/InputFiles/Circuits/adder64.vhdl"))
        ui->lineEdit_CircuitPath->setText("../../../files/InputFiles/Circuits/adder64.vhdl");
    else
        ui->lineEdit_CircuitPath->setText("");
    if(d.exists("../../../files/InputFiles/Cell_Libraries/my_cell_library.hs"))
        ui->lineEdit_CellLibPath->setText("../../../files/InputFiles/Cell_Libraries/my_cell_library.hs");
    else
        ui->lineEdit_CellLibPath->setText("");
}

MainWindow::~MainWindow()
{
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


void MainWindow::on_pushButton_CreateSolver_clicked()
{
    CreateNewSolver();
}

ProcessArgs MainWindow::GetProcessArgsFromUi()
{
    ProcessArgs p;
    p.popSize   = ui->spinBox_PopSize->value();
    p.pe        = ui->doubleSpinBox_Pe->value()*p.popSize;
    p.pm        = ui->doubleSpinBox_Pm->value()*p.popSize;
    p.rho_e     = ui->doubleSpinBox_Rho_e->value();
    p.maxGens   = ui->checkBox_MaxGens->isChecked()
            * ui->spinBox_MaxGens->value();
    p.deadlock   = ui->checkBox_Deadlock->isChecked()
            * ui->spinBox_Deadlock->value();
    p.minDelay   = ui->checkBox_MinDelay->isChecked()
            * ui->doubleSpinBox_MinDelay->value();

    p.criteriaAndOr = ui->radioButton_AndCriteria->isChecked();

    p.circuitFilePath = ui->lineEdit_CircuitPath->text().toStdString();
    p.celllibFilePath = ui->lineEdit_CellLibPath->text().toStdString();


    p.bias = ui->doubleSpinBox_Bias->value();

    return p;
}

void MainWindow::CreateNewSolver()
{
    ProcessArgs p = GetProcessArgsFromUi();
    if(p.celllibFilePath == "" || p.circuitFilePath == "")
    {
        QMessageBox box;
        box.setText("selezionare libreria e circuito");
        return;
    }
    SolverDialog d(p);
    d.show();
    d.exec();
}
