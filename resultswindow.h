#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QDialog>
#include <QVector>
#include "resultstablemodel.h"

namespace Ui {
    class ResultsWindow;
}

class ResultsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsWindow(QWidget *parent = 0);
    ~ResultsWindow();
    void setData(QVector<double> &new_x1, QVector<double> &new_x2, QVector<double> &new_x3, QVector<double> &new_mt, QVector<double> &new_dt);

private:
    Ui::ResultsWindow *ui;
    ResultsTableModel model;
};

#endif // RESULTSWINDOW_H
