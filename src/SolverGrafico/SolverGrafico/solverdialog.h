#ifndef SOLVERDIALOG_H
#define SOLVERDIALOG_H

#include <QDialog>
#include "../../../lib/SingletonMutex/mutex.h"
#include <QVector>
#include <QProcess>

namespace Ui {
class SolverDialog;
}

class SolverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolverDialog(QWidget *parent = nullptr);
    ~SolverDialog();

public slots:
    void Plot();

private slots:
    void on_pushButton_Start_clicked();

private:
    QProcess* p;
    QVector<double> x,y;
    Ui::SolverDialog *ui;
};

#endif // SOLVERDIALOG_H
