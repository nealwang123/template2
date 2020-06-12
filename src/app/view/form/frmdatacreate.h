#ifndef FRMDATACREATE_H
#define FRMDATACREATE_H

#include <QWidget>

class QSqlTableModel;

namespace Ui {
class frmDataCreate;
}

class frmDataCreate : public QWidget
{
    Q_OBJECT

public:
    explicit frmDataCreate(QWidget *parent = 0);
    ~frmDataCreate();

private:
    Ui::frmDataCreate *ui;

    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型

    int maxCount;
    int currentCount;    
    int deviceLogID;
    QTimer *timer;

    QString tableName;
    QString countName;

private:
    QString getSerialNumber();

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void loadData();
    void dataCreate();

private slots:
    void debug(const QString &msg);
    void finsh(bool ok, quint32 count, double msec);

private slots:
    void on_btnConn_clicked();
    void on_btnStart_clicked();
    void on_btnLoad_clicked();
};

#endif // FRMDATACREATE_H
