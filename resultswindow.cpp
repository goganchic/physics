#include "resultswindow.h"
#include "ui_resultswindow.h"

ResultsWindow::ResultsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(&model);
    ui->tableView->setSortingEnabled(false);
    ui->tableView->verticalHeader()->hide();
}

ResultsWindow::~ResultsWindow()
{
    delete ui;
}

void ResultsWindow::setData(QVector<double> &new_x1, QVector<double> &new_x2, QVector<double> &new_x3, QVector<double> &new_mt, QVector<double> &new_dt)
{
    model.setData(new_x1, new_x2, new_x3, new_mt, new_dt);
}
