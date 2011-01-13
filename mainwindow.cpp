#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    avgCurve("AVG")
{
    ui->setupUi(this);
    avgCurve.attach(ui->plot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newPointArrived(double res)
{
    xs.push_back(xs.size());
    ys.push_back(res);
    avgCurve.setData(&xs[0], &ys[0], xs.size());
    ui->plot->replot();
}

void MainWindow::on_startButton_clicked()
{
    int block_size = ui->blockSizeEdit->text().toInt();
    int interval = ui->intervalEdit->text().toInt();
    storage = new StorageThread(block_size);
    generator = new GeneratorThread(interval);
    processor = new ProcessorThread(block_size);

    connect(generator, SIGNAL(newDataGenerated()), storage, SLOT(cacheRawData()));
    connect(processor, SIGNAL(dataProcessed(double)), storage, SLOT(notifyAboutReadyProcessor(double)));
    connect(storage, SIGNAL(storageBlockReady(int*)), processor, SLOT(processData(int*)));
    connect(processor, SIGNAL(dataProcessed(double)), this, SLOT(newPointArrived(double)));

    xs.clear();
    ys.clear();

    processor->start();
    storage->start();
    generator->start();

    ui->startButton->setDisabled(true);
    ui->stopButton->setDisabled(false);
}

void MainWindow::on_stopButton_clicked()
{
    generator->stop();
    generator->wait();
    disconnect(generator, SIGNAL(newDataGenerated()), storage, SLOT(cacheRawData()));
    processor->quit();
    processor->wait();
    disconnect(processor, SIGNAL(dataProcessed(double)), storage, SLOT(notifyAboutReadyProcessor(double)));
    disconnect(storage, SIGNAL(storageBlockReady(int*)), processor, SLOT(processData(int*)));
    disconnect(processor, SIGNAL(dataProcessed(double)), this, SLOT(newPointArrived(double)));
    storage->quit();
    processor->wait();

    delete generator;
    delete processor;
    delete storage;

    ui->startButton->setDisabled(false);
    ui->stopButton->setDisabled(true);
}
