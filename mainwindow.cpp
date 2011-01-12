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
    connect(processor, SIGNAL(dataProcessed(double)), storage, SLOT(notifyAboutReadyProcessor()));
    connect(storage, SIGNAL(storageBlockReady(int*)), processor, SLOT(processData(int*)));
    connect(processor, SIGNAL(dataProcessed(double)), this, SLOT(newPointArrived(double)));

    processor->start();
    storage->start();
    generator->start();
}
