#include "mytablemodel.h"
QMap<int, Qt::CheckState> check_state_map;
int checkColumn =0;//第几列为复选框，从0开始计算

myTableModel::myTableModel(QObject *parent, QSqlDatabase db):
    QSqlTableModel(parent,db)
{
}

bool myTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;


    if (role == Qt::CheckStateRole && index.column() == checkColumn)
    {
        check_state_map[index.row()] = (value == Qt::Checked ? Qt::Checked : Qt::Unchecked);return true;
    }
    else
        return QSqlTableModel::setData(index, value,role);
}

QVariant myTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.column() != checkColumn )
    {
        return QSqlTableModel::data(index, role);
    }
    switch(role)
    {
        case Qt::CheckStateRole:
        if(index.column() == checkColumn)
        {
            if (check_state_map.contains(index.row()))
            {

                return check_state_map[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked;
            }

            return Qt::Unchecked;
        }
        default:
            return QVariant();
    }
    return QVariant();
}

Qt::ItemFlags myTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    if (index.column() == checkColumn)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return QSqlTableModel::flags(index);
    return  Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable;
}
