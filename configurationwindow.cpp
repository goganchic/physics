#include "configurationwindow.h"
#include "ui_configurationwindow.h"
#include <QIntValidator>

ConfigurationWindow::ConfigurationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationWindow)
{
    ui->setupUi(this);

    QValidator *time_val = new QIntValidator(1, 1000, ui->observationTimeEdit);
    ui->observationTimeEdit->setValidator(time_val);

    QValidator *device_val = new QIntValidator(1, 1000000000, ui->deviceBufferSizeEdit);
    ui->deviceBufferSizeEdit->setValidator(device_val);

    QValidator *preprocessor_val = new QIntValidator(1, 1000000000, ui->preprocessorBlockSizeEdit);
    ui->preprocessorBlockSizeEdit->setValidator(preprocessor_val);

    QValidator *discretization_rate_val = new QIntValidator(1, 1000000000, ui->discretizationRateEdit);
    ui->discretizationRateEdit->setValidator(discretization_rate_val);

    QValidator *t1_val = new QIntValidator(1, 10000, ui->t1Edit);
    ui->t1Edit->setValidator(t1_val);

    QValidator *t2_val = new QIntValidator(1, 10000, ui->t2Edit);
    ui->t2Edit->setValidator(t2_val);

    updateProcessorBlockSize();
    updateInterval();
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::on_okButton_clicked()
{
    close();
}

int ConfigurationWindow::getProcessorBlockSize()
{
    int observation_time = ui->observationTimeEdit->text().toInt();
    int preprocessor_block_size = ui->preprocessorBlockSizeEdit->text().toInt();
    int discretization_rate = ui->discretizationRateEdit->text().toInt();

    return observation_time * discretization_rate / preprocessor_block_size;
}

int ConfigurationWindow::getInterval()
{
    int device_buffer_size = ui->deviceBufferSizeEdit->text().toInt();
    int discretization_rate = ui->discretizationRateEdit->text().toInt();
    return device_buffer_size * 1000.0 / discretization_rate;
}

void ConfigurationWindow::updateInterval()
{
    ui->intervalEdit->setText(QString::number(getInterval()));
}

void ConfigurationWindow::updateProcessorBlockSize()
{
    ui->processorBlockSizeEdit->setText(QString::number(getProcessorBlockSize()) + " x 2");
}


void ConfigurationWindow::on_observationTimeEdit_textEdited(QString )
{
    updateProcessorBlockSize();
}


void ConfigurationWindow::on_deviceBufferSizeEdit_textChanged(QString )
{
    updateInterval();
}

void ConfigurationWindow::on_preprocessorBlockSizeEdit_textChanged(QString )
{
    updateProcessorBlockSize();
}

void ConfigurationWindow::on_discretizationRateEdit_textChanged(QString )
{
    updateInterval();
}
