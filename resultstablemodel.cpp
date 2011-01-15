#include "resultstablemodel.h"
#include <QDebug>

ResultsTableModel::ResultsTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QVariant ResultsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Number";
        case 1:
            return "AVG(t1)";
        case 2:
            return "AVG(t2)";
        case 3:
            return "AVG(t1 + t2)";
        case 4:
            return "MT";
        case 5:
            return "DT";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

int ResultsTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return x1.size();
}

int ResultsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant ResultsTableModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "in data";
    if (!index.isValid())
        return QVariant();

    if (index.row() >= x1.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch(index.column())
        {
        case 0:
            return QString("%1").arg(index.row());
        case 1:
            return x1[index.row()];
        case 2:
            return x2[index.row()];
        case 3:
            return x3[index.row()];
        case 4:
            return mt[index.row()];
        case 5:
            return dt[index.row()];
        }
    }
    return QVariant();
}

void ResultsTableModel::setData(QVector<double> &new_x1, QVector<double> &new_x2, QVector<double> &new_x3, QVector<double> &new_mt, QVector<double> &new_dt)
{
    x1 = new_x1;
    x2 = new_x2;
    x3 = new_x3;
    mt = new_mt;
    dt = new_dt;
    emit layoutChanged();
}
