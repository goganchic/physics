#include "configurationwindow.h"
#include "ui_configurationwindow.h"
#include <QIntValidator>
#include <cmath>


QVector<int> getDividers(int a)
{
    QVector<int> result;
    for (int i = 1; i < a; i++)
    {
        if (a % i == 0)
        {
            result.push_back(i);
        }
    }

    return result;
}

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

    updatePreprocessorBlockSize();
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

int ConfigurationWindow::getPreprocessorBlockSize()
{
    return ui->observationTimeEdit->text().toInt() * ui->discretizationRateEdit->text().toInt() / getX();
}

int ConfigurationWindow::getProcessorBlockSize()
{
    return getX();
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

void ConfigurationWindow::updatePreprocessorBlockSize()
{
    ui->preprocessorBlockSizeEdit->setText(QString::number(getPreprocessorBlockSize()));
}

void ConfigurationWindow::on_observationTimeEdit_textEdited(QString )
{
    updatePreprocessorBlockSize();
    updateProcessorBlockSize();
}


void ConfigurationWindow::on_deviceBufferSizeEdit_textChanged(QString )
{
    updateInterval();
}

void ConfigurationWindow::on_discretizationRateEdit_textChanged(QString )
{
    updatePreprocessorBlockSize();
    updateProcessorBlockSize();
    updateInterval();
}

int ConfigurationWindow::getX()
{
    int n = ui->observationTimeEdit->text().toInt() * ui->discretizationRateEdit->text().toInt();
    int x = sqrt(n);
    QVector<int> dividers = getDividers(n);
    int optimum_x = dividers[0];

    for (int i = 0; i < dividers.size(); i++)
    {
        if (abs(x - dividers[i]) < abs(x - optimum_x))
        {
            optimum_x = dividers[i];
        }
    }

    return optimum_x;
}

