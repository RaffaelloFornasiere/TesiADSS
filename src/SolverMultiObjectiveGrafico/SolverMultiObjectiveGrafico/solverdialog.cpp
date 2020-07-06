#include "solverdialog.h"
#include "ui_solverdialog.h"
#include <QtAlgorithms>
#include <algorithm>

SolverDialog::SolverDialog(const SolverArgs &args, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolverDialog)
{
    ui->setupUi(this);

    this->args = args;

    ui->plot->addGraph(0);
    ui->plot->graph(0)->setName("First");
    //ui->plot->graph(0)->setBrush(Qt::blue);

    ui->plot->addGraph();
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->yAxis2->setVisible(true);
    ui->plot->graph(1)->setName("Best");

    //ui->plot->legend->setVisible(true);

    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->setInteraction(QCP::iSelectPlottables, true);
    this->p = nullptr;
    first = 0;
    graphCount = 0;
}



SolverDialog::~SolverDialog()
{
    if(p != nullptr)
        p->terminate();
    delete p;
    delete ui;
}


void SolverDialog::on_checkBox_PrintDataPlot_stateChanged(int arg1)
{
    ui->lineEdit_DataPlotPath->setEnabled(arg1);
}

void SolverDialog::ReadProcessData()
{
    //qDebug() << p->readAllStandardOutput();
    if(!first)
    {
        first= 1;
        QString s  = QString(p->readAllStandardOutput()).split('\n').first();
        qDebug() << "first " <<  s;
        Plot(s.toStdString(), 0);
    }
    QString s  = QString(p->readAllStandardOutput()).split('\n').first();
    qDebug() << s;
    Plot(s.toStdString(), 1);
}


void SolverDialog::Plot(std::string data, size_t graph)
{
    QVector<double> varea;
    QVector<double> vdelay;
    std::stringstream ss(data);
    double area, delay;
    while((ss >> delay) && (ss >> area))
    {
        varea.push_back(area);
        vdelay.push_back(delay);
    }
    ui->plot->graph(graph)->setData(vdelay, varea);
    ui->plot->rescaleAxes();
    ui->plot->replot();

}

void SolverDialog::on_pushButton_Start_clicked()
{
    ui->checkBox_PrintDataPlot->setEnabled(0);
    ui->lineEdit_DataPlotPath->setEnabled(0);

    //qDebug() << QString::fromStdString(args.circuitPath) << " " << QString::fromStdString(args.cellLibPath);
    std::string solverPath = "../../SolverMultiObjective/SolverMultiObjective/main.exe";
    std::string command =
            solverPath +
            " -c " + args.circuitPath +
            " -l " + args.cellLibPath +
            " -p" + std::to_string(args.popSize) +
            " -m" + std::to_string(args.mutationP) +
            " -g" + std::to_string(args.maxGens);

    QDir d(ui->lineEdit_DataPlotPath->text());
    if(d.exists() && ui->lineEdit_DataPlotPath->text() != "")
        command += " -f " + ui->lineEdit_DataPlotPath->text().toStdString();

    if(p != nullptr)
        p->terminate();
    delete p;
    p = new QProcess(this);
    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadProcessData()));
    p->setReadChannel(QProcess::StandardOutput);
    qDebug() << QString::fromStdString(command);
    p->start(QString::fromStdString(command));
    int i = 0;
    while(p->state() == QProcess::ProcessState::NotRunning)
    {
        i++;
        if(i > 1)
        {
            QMessageBox warn(this);
            warn.setText("Nessun risolutore trovato");
            warn.show();
            warn.exec();
            exit(EXIT_FAILURE);
        }
        QMessageBox warn(this);
        warn.setText("Selezionare il risolutore");
        warn.show();
        warn.exec();

        QFileDialog dialog(this);
        QDir currentDir = QDir::currentPath();
        currentDir.cd("../../../");

        dialog.setDirectory(currentDir);
        dialog.setFileMode(QFileDialog::ExistingFile);
        QString fileName;
        if (dialog.exec())
            fileName = dialog.selectedFiles()[0];
        size_t i = command.find((".exe"));
        command.erase(0, i+4);
        command = fileName.toStdString() + command;
        p->start(QString::fromStdString(command));
    }

}

void SolverDialog::on_pushButton_Stop_clicked()
{
    ui->checkBox_PrintDataPlot->setEnabled(1);
    ui->lineEdit_DataPlotPath->setEnabled(ui->checkBox_PrintDataPlot->isChecked());
    if(p != nullptr)
        p->kill();
}

