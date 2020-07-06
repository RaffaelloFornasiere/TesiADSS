#include "mainwindow.h"
#include "ui_mainwindow.h"



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


void MainWindow::on_pushButton_OpenSolver_clicked()
{
    SolverArgs sa = GetSolverArgsFromUi();
    //qDebug() << QString::fromStdString(sa.circuitPath) << " " << QString::fromStdString(sa.cellLibPath);
    if(sa.cellLibPath == "" || sa.circuitPath == "")
    {
        QMessageBox box;
        box.setText("selezionare libreria e circuito");
        return;
    }
    SolverDialog dialog(sa);
    dialog.show();
    dialog.exec();
}

void MainWindow::on_pushButton_OpenCircuit_clicked()
{
    QFileDialog dialog(this);
    QDir currentDir = QDir::currentPath();
    currentDir.cd("../../../files/InputFiles/Circuits");

    dialog.setDirectory(currentDir);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fileName;
    if (dialog.exec())
    {
        if(dialog.selectedFiles().size() != 0)
        {
            fileName = dialog.selectedFiles()[0];
            ui->lineEdit_CircuitPath->setText(fileName);
        }
    }


}

void MainWindow::on_pushButton_OpenLibrary_clicked()
{
    QFileDialog dialog(this);
    QDir currentDir = QDir::currentPath();
    currentDir.cd("../../../files/InputFiles/Cell_Libraries");

    dialog.setDirectory(currentDir);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fileName;
    if (dialog.exec())
    {
        if(dialog.selectedFiles().size() != 0)
        {
            fileName = dialog.selectedFiles()[0];
            ui->lineEdit_CellLibPath->setText(fileName);
        }
    }


}

SolverArgs MainWindow::GetSolverArgsFromUi()
{
    SolverArgs sa;
    sa.maxGens = ui->spinBox_MaxGens->value();
    sa.popSize = ui->spinBox_PopSize->value();
    sa.mutationP = ui->doubleSpinBox_MutationP->value();
    sa.cellLibPath = ui->lineEdit_CellLibPath->text().toStdString();
    sa.circuitPath = ui->lineEdit_CircuitPath->text().toStdString();
    return sa;
}

