#ifndef TEMPERATUREDELEGATE_H
#define TEMPERATUREDELEGATE_H

#include <QWidget>
#include <QtSql>
#include "quiwidget.h"
#include "dbdelegate.h"
namespace Ui {
class TemperatureDelegate;
}

class TemperatureDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit TemperatureDelegate(QWidget *parent = nullptr);
    ~TemperatureDelegate();
    bool createConnection();
    int addRow();
    void setData(int row,int col,QString data);
private slots:
    void initForm(QString fileName);
public slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnDelete_clicked();
    void on_btnReturn_clicked();
    void on_btnClear_clicked();
private:
    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型
    QSqlDatabase _db;
private:
    Ui::TemperatureDelegate *ui;
};

#endif // TEMPERATUREDELEGATE_H
