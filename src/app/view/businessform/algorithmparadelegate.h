﻿#ifndef ALGORITHMPARADELEGATE_H
#define ALGORITHMPARADELEGATE_H

#include <QWidget>
#include <QtSql>
#include "quiwidget.h"
#include "dbdelegate.h"
namespace Ui {
class AlgorithmParaDelegate;
}

class AlgorithmParaDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit AlgorithmParaDelegate(QWidget *parent = nullptr);
    ~AlgorithmParaDelegate();
    bool createConnection();
    int addRow();
    int getRow();
    int getType(int row);
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
    Ui::AlgorithmParaDelegate *ui;
};

#endif // ALGORITHMPARADELEGATE_H
