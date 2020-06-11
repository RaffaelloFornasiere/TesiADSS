#include "solverdialog.h"
#include "ui_solverdialog.h"
#include <QProcess>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>

SolverDialog::SolverDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolverDialog)
{
    ui->setupUi(this);
}

SolverDialog::~SolverDialog()
{
    delete ui;
}

void SolverDialog::Plot()
{
    qDebug() << p->readAllStandardOutput();
}

void SolverDialog::on_pushButton_Start_clicked()
{
    const char* command = "../../Solver/CircuitSolver/main 	-c ../../../files/InpuFiles/Circuits/adder16.vhdl \
                                    -l ../../../files/InputFiles/Cell_Libraries/my_cell_library.hs \
                                    -f \
                                    -p 100 \
                                    -e 20 \
                                    -m 30 \
                                    -r 0.7 \
                                    -s 1";

    p = new QProcess(this);
    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(Plot()));
    p->setReadChannel(QProcess::StandardOutput);
    p->start(command);
    //p->waitForFinished()

}
