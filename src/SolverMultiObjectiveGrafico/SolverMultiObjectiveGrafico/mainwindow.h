#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "solverdialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_OpenSolver_clicked();
    void on_pushButton_OpenCircuit_clicked();
    void on_pushButton_OpenLibrary_clicked();

private:

    SolverArgs GetSolverArgsFromUi();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
