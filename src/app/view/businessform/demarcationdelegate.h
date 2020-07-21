#ifndef DEMARCATIONDELEGATE_H
#define DEMARCATIONDELEGATE_H

#include <QWidget>
#include <QtSql>
#include <QtConcurrent>
#include "zhelpers.hpp"
#include "mysubclassedsqltablemodel.h"
namespace Ui {
class DemarcationDelegate;
}
#define SQLDATAINDEX 7
class DemarcationDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit DemarcationDelegate(QWidget *parent = 0);
    ~DemarcationDelegate();
    bool createConnection();
    void startDemarcation(QStringList list);
    //function:替换数据功能函数
    void replaceStr(int rowIndex,int index,int len,QString replaceStr);
    void getDataByIndex(int index,QString &send_data,QString &recv_data);
    void setData(int row,int clown,QString data);
private:
    Ui::DemarcationDelegate *ui;

    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型

    QSqlDatabase _db;
    QStringList m_deviceInfoList;
    QTimer* demarcationTimer;
    int demarcationIndex;


private slots:
    void initForm(QString fileName);
    void slot_demarcationTimer();


private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnDelete_clicked();
    void on_btnReturn_clicked();
    void on_btnClear_clicked();
    void on_pushButton_clicked();
};

#endif // DEMARCATIONDELEGATE_H
