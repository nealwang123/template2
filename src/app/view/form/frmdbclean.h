#ifndef FRMDBCLEAN_H
#define FRMDBCLEAN_H

#include <QWidget>

class QSqlTableModel;

namespace Ui {
class frmDbClean;
}

class frmDbClean : public QWidget
{
    Q_OBJECT

public:
    explicit frmDbClean(QWidget *parent = 0);
    ~frmDbClean();

private:
    Ui::frmDbClean *ui;

    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型

    int maxCount;
    int currentCount;    
    int deviceLogID;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void loadData();

private slots:
    void debug(const QString &msg);
    void cleanFinsh(bool ok, int cleanCount, double msec);

private slots:
    void on_btnConn_clicked();
    void on_btnStart_clicked();
    void on_btnLoad_clicked();
};

#endif // FRMDBCLEAN_H
