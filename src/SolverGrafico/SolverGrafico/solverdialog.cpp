#include "solverdialog.h"
#include "ui_solverdialog.h"


SolverDialog::SolverDialog(const ProcessArgs &p, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolverDialog)
{

    ui->setupUi(this);
    args = p;
    ui->label_PopSize->setText(QString::number(args.popSize));
    ui->label_Pe->setText(QString::number(args.pe));
    ui->label_Pm->setText(QString::number(args.pm));
    ui->label_Rho_e->setText(QString::number(args.rho_e));

    size_t pos = p.circuitFilePath.find_last_of('/');
    QString s = QString::fromStdString(p.circuitFilePath.substr(pos+1));
    ui->label_CircuitFile->setText(s);
    pos = p.celllibFilePath.find_last_of('/');
    s = QString::fromStdString(p.celllibFilePath.substr(pos+1));
    ui->label_CellLib->setText(s);

    ui->plot->addGraph(0);
    ui->plot->graph(0)->setName("Delay");
    //ui->plot->graph(0)->setBrush(Qt::blue);

    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis2);
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->yAxis2->setVisible(true);
    ui->plot->graph(1)->setName("Area");

    ui->plot->legend->setVisible(true);

    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->setInteraction(QCP::iSelectPlottables, true);
    this->p = nullptr;

    first = false;

}

SolverDialog::~SolverDialog()
{
    if(p != nullptr)
        p->terminate();
    delete ui;
}

void SolverDialog::Plot(double x, double y, double a)
{
    ui->plot->graph(0)->addData(x, y);
    ui->plot->graph(1)->addData(x, a);
    ui->plot->yAxis2->rescale();
    ui->plot->xAxis->rescale();
    ui->plot->replot();
}

void SolverDialog::ReadProcessData()
{
    QStringList list = QString(p->readAllStandardOutput()).split('\n');

    for(auto& aux : list)
    {
        if(aux == "")
            continue;
        else if(aux.contains("WARNING"))
        {
            std::string received = aux.toStdString();
            size_t pos = 0;
            if((pos = received.find("WARNING")) != std::string::npos)
            {
                std::string warn = received;
                size_t pos = warn.find("WARNING");
                warn.substr(pos, warn.find("\n", pos)-pos);
                ui->label_Warnings->setText(QString::fromStdString(warn));
            }
        }
        else if(aux.contains("d:"))
        {
            std::string received = aux.toStdString();
            size_t dpos = received.find("d:") + 2;
            size_t apos = received.find("a:");
            size_t tpos = received.find("t:");
            if(tpos == std::string::npos)
                qDebug() << "formattazione errata" << aux;
            else
            {
                //qDebug() << std::stod(received.substr(dpos, apos-dpos));
                double y = std::stod(received.substr(dpos, apos-dpos));
                //qDebug() <<std::stod(received.substr(tpos+2));
                double x = std::stod(received.substr(tpos+2));
                //qDebug() << std::stod(received.substr(apos+2, tpos - (apos+2)));
                double a = std::stod(received.substr(apos+2, tpos -(apos+2)));
                if(!ui->label_BestSolution->text().contains(QString::number(y)))
                {
                    ui->label_BestSolution->setText("Delay: " + QString::number(y)
                                                    + " area:" + QString::number(a)
                                                    + " time:" + QString::number(x));
                }
                Plot(x,y,a);
                if(!first)
                {
                    first = 1;
                    ui->label_BeginSolution->setText("Delay: " + QString::number(y)
                                                     + " area:" + QString::number(a));
                }
            }
        }
        else
        {
            qDebug() << aux;
        }

    }

}

void SolverDialog::on_pushButton_Start_clicked()
{
    first = 0;
    ui->checkBoxPlotOutput->setEnabled(0);
    std::string programPath = "./../../Solver/CircuitSolver/main.exe";
    std::string command =
            programPath +
            " -c " +
            args.circuitFilePath +
            " -l " +
            args.celllibFilePath +
            " -f . -p " +
            std::to_string(args.popSize) +
            " -e " +
            std::to_string(args.pe) +
            " -m " +
            std::to_string(args.pm) +
            " -r " +
            std::to_string(args.rho_e) +
            " -b " +
            std::to_string(args.bias);
    if(args.maxGens)
        command += " -g " + std::to_string(args.maxGens);
    if(args.deadlock)
        command += " -d " + std::to_string(args.deadlock);
    if(args.minDelay)
        command += " -t " + std::to_string(args.minDelay);
    if(args.criteriaAndOr)
        command += " -a";
    if(ui->checkBoxPlotOutput->isChecked())
        command += " -n";

    if(p != nullptr)
        p->terminate();
    p = new QProcess(this);
    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadProcessData()));
    p->setReadChannel(QProcess::StandardOutput);
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
    ui->checkBoxPlotOutput->setEnabled(1);
    if(p != nullptr)
        p->kill();
}

void SolverDialog::on_checkBox_AreaPlot_stateChanged(int arg1)
{
    ui->plot->graph(1)->setVisible(arg1);
}
