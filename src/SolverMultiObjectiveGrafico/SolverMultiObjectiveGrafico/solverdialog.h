#ifndef SOLVERDIALOG_H
#define SOLVERDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QTextStream>
#include <sstream>

struct SolverArgs
{
    std::string circuitPath;
    std::string cellLibPath;

    size_t popSize;
    double mutationP;
    size_t maxGens;
};

namespace Ui {
class SolverDialog;
}

class SolverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolverDialog(const SolverArgs& args, QWidget *parent = nullptr);
    ~SolverDialog();

private slots:
    void on_checkBox_PrintDataPlot_stateChanged(int arg1);
    void ReadProcessData();
    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();

private:
    void Plot(std::string data, size_t graph);
    bool first;
    QProcess* p;
    size_t graphCount;

    SolverArgs args;
    Ui::SolverDialog *ui;
};

#endif // SOLVERDIALOG_H
