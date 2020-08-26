#ifndef CANCONFIGDELEGATE_H
#define CANCONFIGDELEGATE_H

#include <QWidget>
#include <QtSql>
#include "quiwidget.h"
#include "dbdelegate.h"
namespace Ui {
class CANConfigDelegate;
}

class CANConfigDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit CANConfigDelegate(QWidget *parent = nullptr);
    ~CANConfigDelegate();
    bool createConnection();
    QStringList getModelData(int index);
private:
    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型
    QSqlDatabase _db;

private slots:
    void initForm(QString fileName);
private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnDelete_clicked();
    void on_btnReturn_clicked();
    void on_btnClear_clicked();
private:
    Ui::CANConfigDelegate *ui;

};

#endif // CANCONFIGDELEGATE_H
