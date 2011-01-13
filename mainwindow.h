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
    QwtPlotCurve avgCurve;
    QVector<double> xs;
    QVector<double> ys;

public slots:
    void newPointArrived(double res);


private slots:
    void on_stopButton_clicked();
    void on_startButton_clicked();

private:
    GeneratorThread *generator;
    ProcessorThread *processor;
    StorageThread *storage;
};

#endif // MAINWINDOW_H
