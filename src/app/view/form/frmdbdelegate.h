#ifndef FRMDBDELEGATE_H
#define FRMDBDELEGATE_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class frmDbDelegate;
}

class frmDbDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit frmDbDelegate(QWidget *parent = 0);
    ~frmDbDelegate();
    bool createConnection();

private:
    Ui::frmDbDelegate *ui;

    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型

    QSqlDatabase _db;

private slots:
    void initForm();


private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnDelete_clicked();
    void on_btnReturn_clicked();
    void on_btnClear_clicked();
};

#endif // FRMDBDELEGATE_H
