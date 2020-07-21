#ifndef MYSUBCLASSEDSQLTABLEMODEL_H
#define MYSUBCLASSEDSQLTABLEMODEL_H
#include <QSqlTableModel>
#include <QObject>
#include <QColor>
class MySubClassedSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
    public:
       MySubClassedSqlTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase())
       : QSqlTableModel(parent,db) {}
       QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const
       {
          if(role==Qt::TextColorRole)
          {
             const QVariant value(data(index,Qt::DisplayRole));
             if(value.toString()=="PASS"){
                 return QVariant(QColor(Qt::green));
             }else if(value.toString()=="FAIL"){
                return QVariant(QColor(Qt::red));
             }

          }
          return QSqlTableModel::data(index,role);
       }
};

#endif // MYSUBCLASSEDSQLTABLEMODEL_H
