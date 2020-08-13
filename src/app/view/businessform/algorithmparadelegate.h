#ifndef ALGORITHMPARADELEGATE_H
#define ALGORITHMPARADELEGATE_H

#include <QWidget>
#include <QtSql>
#include "quiwidget.h"
#include "dbdelegate.h"
#include "mytablemodel.h"
namespace Ui {
class AlgorithmParaDelegate;
}
#define SENDENABLEINDEX 1
#define SENDINDEX 2
#define RAWINDEX 3
#define REALINDEX 4
#define COMPAREINDEX 5
#define TYPEINDEX 6
static bool createConnectionDateBase();
class AlgoPara{
    public:
    int _type;//类型
    int _index;
    QString enable;
    QString rawstr;
    QString realstr;
    QString comparestr;
    QString description;
};
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

    QList<AlgoPara > debugData();
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
    //myTableModel *model;      //数据表格模型
    QSqlTableModel *model;
    QSqlDatabase _db;
private:
    Ui::AlgorithmParaDelegate *ui;
    QList<AlgoPara>listPara;
};

#endif // ALGORITHMPARADELEGATE_H
