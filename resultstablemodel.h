#ifndef RESULTSTABLEMODEL_H
#define RESULTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

class ResultsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ResultsTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(QVector<double> &new_x1, QVector<double> &new_x2, QVector<double> &new_x3, QVector<double> &new_mt, QVector<double> &new_dt);

private:
    QVector<double> x1;
    QVector<double> x2;
    QVector<double> x3;
    QVector<double> mt;
    QVector<double> dt;
};

#endif // RESULTSTABLEMODEL_H
