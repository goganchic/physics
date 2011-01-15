#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <qwt_plot_curve.h>
#include "generatorthread.h"
#include "processorthread.h"
#include "storagethread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QwtPlotCurve avgCurve1;
    QwtPlotCurve avgCurve2;
    QwtPlotCurve avgCurve3;
    QwtPlotCurve mtCurve;
    QwtPlotCurve dtCurve;
    QVector<double> xs1;
    QVector<double> ys1;
    QVector<double> xs2;
    QVector<double> ys2;
    QVector<double> xs3;
    QVector<double> ys3;
    QVector<double> xsmt;
    QVector<double> ysmt;
    QVector<double> xsdt;
    QVector<double> ysdt;

public slots:
    void newPointArrived(double res1, double res2, double res3, double mt, double dt);
    void usedMemoryChanged(int blocks_count, int size);


private slots:
    void on_stopButton_clicked();
    void on_startButton_clicked();

private:
    GeneratorThread *generator;
    ProcessorThread *processor;
    StorageThread *storage;
};

#endif // MAINWINDOW_H
