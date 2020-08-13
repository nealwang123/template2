#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QtSql>
extern QMap<int, Qt::CheckState> check_state_map;
extern int checkColumn ;//第几列为复选框，从0开始计算
class myTableModel:public QSqlTableModel
{
    Q_OBJECT
public:
    myTableModel( QObject * parent = 0, QSqlDatabase db = QSqlDatabase() );

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

};

#endif // MYTABLEMODEL_H
