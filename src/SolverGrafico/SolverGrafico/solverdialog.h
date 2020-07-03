#ifndef SOLVERDIALOG_H
#define SOLVERDIALOG_H

#include <QDialog>
#include <QVector>
#include <QProcess>
#include <QDebug>
#include <QThread>

struct ProcessArgs
{
    //parameters
    size_t popSize;
    size_t pe;
    size_t pm;
    double rho_e;

    //stop criteria
    size_t maxGens;
    size_t deadlock;
    double minDelay;

    double bias;
    std::string circuitFilePath;
    std::string celllibFilePath;

    bool criteriaAndOr;
};


namespace Ui {
class SolverDialog;
}

class SolverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolverDialog(const ProcessArgs& p, QWidget *parent = nullptr);
    ~SolverDialog();

public slots:
    void Plot(double x, double y, double a);
    void ReadProcessData();

private slots:
    void on_pushButton_Start_clicked();

    void on_pushButton_Stop_clicked();

    void on_checkBox_AreaPlot_stateChanged(int arg1);

private:
    bool plotArea;
    bool first ;
    ProcessArgs args;
    QProcess* p;
    QVector<double> x,y;
    Ui::SolverDialog *ui;
};

#endif // SOLVERDIALOG_H
