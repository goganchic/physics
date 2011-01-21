#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qwt_legend.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    avgCurve1("AVG(t1)"),
    avgCurve2("AVG(t2)"),
    avgCurve3("AVG(t1 + t2)"),
    mtCurve("MT"),
    dtCurve("DT"),
    resultsWindow(this)
{
    ui->setupUi(this);

    avgCurve1.setPen(QPen(Qt::red));
    avgCurve2.setPen(QPen(Qt::green));
    avgCurve3.setPen(QPen(Qt::blue));
    mtCurve.setPen(QPen(Qt::darkYellow));

    avgCurve1.setRenderHint(QwtPlotItem::RenderAntialiased);
    avgCurve2.setRenderHint(QwtPlotItem::RenderAntialiased);
    avgCurve3.setRenderHint(QwtPlotItem::RenderAntialiased);
    mtCurve.setRenderHint(QwtPlotItem::RenderAntialiased);
    dtCurve.setRenderHint(QwtPlotItem::RenderAntialiased);

    avgCurve1.attach(ui->plot);
    avgCurve2.attach(ui->plot);
    avgCurve3.attach(ui->plot);
    mtCurve.attach(ui->plot);
    dtCurve.attach(ui->plot);

    ui->plot->setAxisTitle(QwtPlot::xBottom, "time -->");
    ui->plot->setAxisTitle(QwtPlot::yLeft, "value -->");
    ui->plot->insertLegend(new QwtLegend(), QwtPlot::TopLegend);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newPointArrived(double res1, double res2, double res3, double mt, double dt)
{
    xs1.push_back(xs1.size());
    ys1.push_back(res1);
    avgCurve1.setData(&xs1[0], &ys1[0], xs1.size());

    xs2.push_back(xs2.size());
    ys2.push_back(res2);
    avgCurve2.setData(&xs2[0], &ys2[0], xs2.size());

    xs3.push_back(xs3.size());
    ys3.push_back(res3);
    avgCurve3.setData(&xs3[0], &ys3[0], xs3.size());

    xsmt.push_back(xsmt.size());
    ysmt.push_back(mt);
    mtCurve.setData(&xsmt[0], &ysmt[0], xsmt.size());

    xsdt.push_back(xsdt.size());
    ysdt.push_back(dt);
    dtCurve.setData(&xsdt[0], &ysdt[0], xsdt.size());

    ui->plot->replot();
}

void MainWindow::usedMemoryChanged(int blocks_count, int size, int temp_buffer_position)
{
    ui->blocksCountEdit->setText(QString::number(blocks_count));
    ui->memoryUsageEdit->setText(QString::number(size));
    ui->tempBufferPositionEdit->setText(QString::number(temp_buffer_position));
}

void MainWindow::on_startButton_clicked()
{
    int observation_time = ui->observationTimeEdit->text().toInt();
    int device_buffer_size = ui->deviceBufferSizeEdit->text().toInt();
    int discretization_rate = ui->discretizationRateEdit->text().toInt();
    int t1 = ui->t1Edit->text().toInt();
    int t2 = ui->t2Edit->text().toInt();
    int interval = device_buffer_size * 1000.0 / discretization_rate;
    int block_size = observation_time * discretization_rate;

    storage = new StorageThread(block_size, device_buffer_size);
    processor = new ProcessorThread(block_size, t1, t2);

    connect(&generator, SIGNAL(timeout()), storage, SLOT(cacheRawData()));
    connect(processor, SIGNAL(processorReady()), storage, SLOT(notifyAboutReadyProcessor()));
    connect(storage, SIGNAL(storageBlockReady(short*)), processor, SLOT(processData(short*)));
    connect(processor, SIGNAL(dataProcessed(double, double, double, double, double)), this, SLOT(newPointArrived(double, double, double, double, double)));
    connect(storage, SIGNAL(changeUsedMemory(int,int, int)), this, SLOT(usedMemoryChanged(int,int, int)));

    xs1.clear();
    ys1.clear();
    xs2.clear();
    ys2.clear();
    xs3.clear();
    ys3.clear();
    xsmt.clear();
    ysmt.clear();
    xsdt.clear();
    ysdt.clear();

    processor->start();
    storage->start();
    generator.start(interval);

    ui->startButton->setDisabled(true);
    ui->stopButton->setDisabled(false);
}

void MainWindow::on_stopButton_clicked()
{
    generator.stop();
    disconnect(&generator, SIGNAL( timeout()), storage, SLOT(cacheRawData()));

    processor->quit();
    processor->wait();
    disconnect(processor, SIGNAL(processorReady()), storage, SLOT(notifyAboutReadyProcessor()));
    disconnect(storage, SIGNAL(storageBlockReady(short*)), processor, SLOT(processData(short*)));
    disconnect(processor, SIGNAL(dataProcessed(double, double, double, double, double)), this, SLOT(newPointArrived(double, double, double, double, double)));
    disconnect(storage, SIGNAL(changeUsedMemory(int,int, int)), this, SLOT(usedMemoryChanged(int,int, int)));
    storage->quit();
    storage->wait();

    delete processor;
    delete storage;

    ui->startButton->setDisabled(false);
    ui->stopButton->setDisabled(true);

    resultsWindow.setData(ys1, ys2, ys3, ysmt, ysdt);
    resultsWindow.show();
}
